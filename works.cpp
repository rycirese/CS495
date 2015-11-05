//Texture Maze Test Code

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
//MAC INCLUDES
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#endif
//WINDOWS INCLUDES
#ifdef _WIN32
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#endif
using namespace std;

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 400
#define PI 3.1415926535897932384626433832795

SDL_Window *mw;

//Build Vertex Structure
typedef struct{
    float x, y, z; //3D Coordinates
    float u, v; //Texture Coordinates
} vertex;

//Build Triangle Structure
typedef struct{
    vertex vertex[3]; //Array Of Three Vertices
} triangle;

//Build Sector Structure
typedef struct{
    int numTriangles; //Number Of Triangles In Sector
    triangle *triangle; //Pointer To Array Of Triangles
} sector;

sector sector1; //Our sector

GLfloat yrot; //Camera rotation variable
GLfloat xpos, zpos; //Camera pos variable

GLfloat walkbias, walkbiasangle; //Head-bobbing variables
GLfloat lookupdown;

GLfloat LightAmbient[]  = { 1.0f, 0.0f, 0.0f, 1.0f }; //Ambient Light Values
GLfloat LightDiffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; //Diffuse Light Values
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f }; //Light Position, positive z is between the screen and me

const float piover180 = 0.0174532925f; //Constant used for converting to radians

GLuint filter; //Which Filter To Use
GLuint texture[3]; //Storage for 3 textures

//Function to release/destroy our resources and restoring the old desktop
void Quit(int returnCode){
    SDL_Quit(); //clean up the window
    if(sector1.triangle) free(sector1.triangle); //Deallocate things we allocated
    exit(returnCode); //And exit appropriately
}

//Function to load in bitmap as a GL texture
int LoadGLTextures(){
    bool Status = false;

    SDL_Surface *TextureImage[1];

    TextureImage[0] = SDL_LoadBMP("data/mud.bmp");
    
    Status = true; //Set the status to true

    //TEXTURE 1
    glGenTextures(3, &texture[0]); //Create The Texture
    glBindTexture(GL_TEXTURE_2D, texture[0]); //Load in texture 1
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->w, TextureImage[0]->h, 0, GL_BGR, GL_UNSIGNED_BYTE, TextureImage[0]->pixels); //Generate The Texture
    //Nearest Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //TEXTURE 2
    glBindTexture(GL_TEXTURE_2D, texture[1]); //Load in texture 2
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->w, TextureImage[0]->h, 0, GL_BGR, GL_UNSIGNED_BYTE, TextureImage[0]->pixels); //Generate The Texture
    //Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //TEXTURE 3
    glBindTexture(GL_TEXTURE_2D, texture[2]); //Load in texture 3
    //Mipmapped Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    SDL_FreeSurface(TextureImage[0]);
    return Status;
}

//Read In A String
void readstr(FILE *f, char *string){
    do fgets( string, 255, f ); //Read One Line
    while((string[0] == '/')||(string[0] == '\n'));
    return;
}

//Setup World
void SetupWorld(string worldFile){
    FILE *filein; //File To Work With

    int numTriangles; //Number of Triangles
    char oneLine[255]; //One line from conf file

    float x, y, z, u, v; //3d and texture coordinates

    int triLoop; //Triangle loop variable
    int verLoop; //Vertex loop variable

    filein = fopen(worldFile.c_str(), "rt"); //Open file
    readstr(filein, oneLine); //Grab a line from 'filein'
    sscanf(oneLine, "NUMPOLLIES %d\n", &numTriangles); //Read in number of triangle

    //Allocate space for triangles
    sector1.triangle = (triangle*)malloc(numTriangles*sizeof(triangle));
    if(sector1.triangle == NULL){
	    fprintf(stderr, "Could not allocate memory for triangles.\n");
	    Quit(1);
	}
    sector1.numTriangles = numTriangles;

    //Get coords for each triangle
    for ( triLoop = 0; triLoop < numTriangles; triLoop++ ){
	    for (verLoop = 0; verLoop < 3; verLoop++){
		    readstr(filein, oneLine);
		    sscanf(oneLine, "%f %f %f %f %f\n", &x, &y, &z, &u, &v);
		    sector1.triangle[triLoop].vertex[verLoop].x = x;
		    sector1.triangle[triLoop].vertex[verLoop].y = y;
		    sector1.triangle[triLoop].vertex[verLoop].z = z;
		    sector1.triangle[triLoop].vertex[verLoop].u = u;
		    sector1.triangle[triLoop].vertex[verLoop].v = v;
		}
	}
    fclose(filein); //Close file
    return;
}

//Function to reset our viewport after a window resize
int resizeWindow(int width, int height){
    GLfloat ratio; //Height / width ration
    if(height == 0) height = 1; //Protect against a divide by zero
    ratio = (GLfloat)width/(GLfloat)height;

    glViewport(0, 0, (GLint)width, (GLint)height); //Setup viewport
    glMatrixMode(GL_PROJECTION); //Change to the projection matrix and set our viewing volume.
    glLoadIdentity();
  	glFrustum(-tan(45.0/360*PI)*0.1*ratio, tan(45.0/360*PI)*0.1*ratio, -tan(45.0/360*PI)*0.1, tan(45.0/360*PI)*0.1, 0.1, 100); //Set perspective
    glMatrixMode(GL_MODELVIEW); //Make sure we're chaning the model view and not the projection
    glLoadIdentity(); //Reset The View

    return true;
}

//Function to handle key press events
void handleKeyPress(SDL_Keysym *keysym){
    switch(keysym->sym){
        case SDLK_ESCAPE: //ESC key closes program
            Quit(0);
            break;
        case SDLK_F1: //F1 key toggles fullscreen mode
    	    //SDL_WM_ToggleFullScreen( surface );
            break;
        case SDLK_RIGHT: //Right arrow key effectively turns the camera right by rotating the scene left
            yrot -= 1.5f;
            break;
        case SDLK_LEFT: //Left arrow key effectively turns the camera left by rotating the scene right
            yrot += 1.5f;
            break;
        case SDLK_UP: //Up arrow moves the player forward
            xpos -= (float)sin(yrot*piover180)*0.05f; //on the x-plane based on Player Direction
            zpos -= (float)cos(yrot*piover180)*0.05f; //on the z-plane based on Player Direction
            if (walkbiasangle >= 359.0f) walkbiasangle = 0.0f;
            else walkbiasangle += 10;
            
            walkbias = (float)sin(walkbiasangle*piover180)/20.0f; //Causes the player to bounce
            break;
        case SDLK_DOWN: //Down arrow key moves the player backwards
            xpos += (float)sin(yrot*piover180)*0.05f;
            zpos += (float)cos(yrot*piover180)*0.05f;
            if(walkbiasangle <= 1.0f) walkbiasangle = 359.0f;
            else walkbiasangle -= 10;
            walkbias = (float)sin(walkbiasangle*piover180)/20.0f;
            break;
        default:
            break;
	}
    return;
}

//General OpenGL Initialization Function
int initGL(){
    //Load in the texture
    if(!LoadGLTextures()) return false;
    glEnable(GL_TEXTURE_2D); //Enable Texture Mapping
    glShadeModel(GL_SMOOTH); //Enable smooth shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Set the background black
    glClearDepth(1.0f); //Depth buffer setup
    glEnable(GL_DEPTH_TEST); //Enables Depth Testing
    glDepthFunc(GL_LEQUAL); //The Type Of Depth Test To Do
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST ); //Really Nice Perspective Calculations

    //LIGHT
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); //Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); //Setup The Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); //Position The Light
    glEnable(GL_LIGHT1); //Enable Light One

    lookupdown    = 0.0f;
    walkbias      = 0.0f;
    walkbiasangle = 0.0f;

    glColor4f( 1.0f, 1.0f, 1.0f, 0.5f); //Full Brightness, 50% Alpha
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Blending Function For Translucency Based On Source Alpha Value

    return true;
}

int drawGLScene(GLvoid){
    //These alculate fps
    static GLint T0 = 0;
    static GLint Frames = 0;

    GLfloat x_m, y_m, z_m, u_m, v_m; //Floating Point For Temp X, Y, Z, U And V Vertices
    GLfloat xtrans = -xpos; //Used For Player Translation On The X Axis
    GLfloat ztrans = -zpos; //Used For Player Translation On The Z Axis
    GLfloat ytrans = -walkbias-0.25f; //Used For Bouncing Motion Up And Down
    GLfloat sceneroty = 360.0f-yrot; //360 Degree Angle For Player Direction

    int loop_m; //Loop variable

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear The Screen And The Depth Buffer
    glLoadIdentity();

    glRotatef(lookupdown, 1.0f, 0.0f , 0.0f); //Rotate Up And Down To Look Up And Down
    glRotatef(sceneroty, 0.0f, 1.0f , 0.0f); //Rotate Depending On Direction Player Is Facing
    glTranslatef(xtrans, ytrans, ztrans); //Translate The Scene Based On Player Position
    glBindTexture(GL_TEXTURE_2D, texture[filter]); //Select A Texture Based On filter

    //Process Each Triangle
    for(loop_m = 0; loop_m < sector1.numTriangles; loop_m++){
	    glBegin(GL_TRIANGLES); //Start Drawing Triangles
	      glNormal3f( 0.0f, 0.0f, 1.0f); //Normal Pointing Forward
	      x_m = sector1.triangle[loop_m].vertex[0].x; //X Vertex Of 1st Point
	      y_m = sector1.triangle[loop_m].vertex[0].y; //Y Vertex Of 1st Point
	      z_m = sector1.triangle[loop_m].vertex[0].z; //Z Vertex Of 1st Point
	      u_m = sector1.triangle[loop_m].vertex[0].u; //U Texture Coord Of 1st Point
	      v_m = sector1.triangle[loop_m].vertex[0].v; //V Texture Coord Of 1st Point

	      //Set The TexCoord And Vertice
	      glTexCoord2f(u_m, v_m);
	      glVertex3f(x_m, y_m, z_m);

	      x_m = sector1.triangle[loop_m].vertex[1].x; //X Vertex Of 2nd Point
	      y_m = sector1.triangle[loop_m].vertex[1].y; //Y Vertex Of 2nd Point
	      z_m = sector1.triangle[loop_m].vertex[1].z; //Z Vertex Of 2nd Point
	      u_m = sector1.triangle[loop_m].vertex[1].u; //U Texture Coord Of 2nd Point
	      v_m = sector1.triangle[loop_m].vertex[1].v; //V Texture Coord Of 2nd Point

	      //Set The TexCoord And Vertice
	      glTexCoord2f(u_m, v_m);
	      glVertex3f(x_m, y_m, z_m);

	      x_m = sector1.triangle[loop_m].vertex[2].x; //X Vertex Of 3rd Point
	      y_m = sector1.triangle[loop_m].vertex[2].y; //Y Vertex Of 3rd Point
	      z_m = sector1.triangle[loop_m].vertex[2].z; //Z Vertex Of 3rd Point
          u_m = sector1.triangle[loop_m].vertex[2].u; //Texture Coord Of 3rd Point
	      v_m = sector1.triangle[loop_m].vertex[2].v; //V Texture Coord Of 3rd Point

	      //Set The TexCoord And Vertice
	      glTexCoord2f(u_m, v_m);
	      glVertex3f(x_m, y_m, z_m);
	    glEnd();
	}
    SDL_GL_SwapWindow (mw); //Draw it to the screen

    //Gather our frames per second
    Frames++;{
        GLint t = SDL_GetTicks(); //How many times the clock has ticked since the beginning of the program
        
        //We imagine the clock ticks 1000 times per second.
        if (t-T0 >= 5000){
            GLfloat seconds = (t-T0)/1000.0;
            GLfloat fps = Frames/seconds;
            printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
            T0 = t;
            Frames = 0;
        }
    }
    return true;
}

int main(int argc, char **argv){
    bool done = false;
    SDL_Event event;
    bool isActive = true;
    SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	mw = SDL_CreateWindow ("Maze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
	SDL_GL_CreateContext(mw); //Associates the OpenGL commands to window w.
	SDL_GL_SetSwapInterval(1);

    initGL();

    SetupWorld( "data/world.txt" );

    resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );

    while(!done){
	    while(SDL_PollEvent(&event)){
		    switch(event.type){
                case SDL_KEYDOWN:
                    handleKeyPress(&event.key.keysym);
                    break;
                case SDL_QUIT: //handle quit requests
                    done = true;
                    break;
                default:
                    break;
			}
		}
	    if(isActive) drawGLScene();
	}
    Quit(0);
    return 0;
}
