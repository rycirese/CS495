//Texture Maze Test Code
#include "World.h"


///////////////////////////////////////////////////////////
//                   GLOBAL VARIABLES                    //
SDL_Surface **TI;                                        //
GLuint texture[3]; //Storage for 3 Textures              //
GLuint filter; //Which Filter To Use                     //
                                                         //
sector sector1; //Our Sector                             //
                                                         //
GLfloat yrot; //Camera rotation variable                 //
GLfloat xpos, zpos; //Camera pos variable                //
GLfloat walkbias, walkbiasangle; //Head-bobbing variables//
GLfloat lookupdown;                                      //
///////////////////////////////////////////////////////////


//Main Function, Creates World
void genWorld(){
    initGL(); //General OpenGL Initialization Function
    SetupWorld("data/world.txt"); //Text File Holding World Layout
    resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT); //Reset Viewport After a Window Resize
}

void initGL(){
    GLfloat LightAmbient[]  = { 1.0f, 0.0f, 0.0f, 1.0f }; //Ambient Light Values
    GLfloat LightDiffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; //Diffuse Light Values
    GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f }; //Light Position, positive z is between the screen and me
    
    //Load in the texture
    LoadGLTextures();
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
}

//Function to load in bitmap as a GL texture
void LoadGLTextures(){
    TI = new SDL_Surface *[6];
    TI[0] = IMG_Load("data/iron.jpg");

    //TEXTURE 1
    glGenTextures(3, &texture[0]); //Create The Texture
    glBindTexture(GL_TEXTURE_2D, texture[1]); //Load in texture 1
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TI[0]->w, TI[0]->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, TI[0]->pixels); //Generate The Texture
    //Nearest Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(TI[0]);
}

//Setup World
void SetupWorld(string worldFile){
    FILE *filein = fopen(worldFile.c_str(), "rt"); //File To Work With

    int numTriangles; //Number of Triangles
    char oneLine[255]; //One line from conf file

    float x, y, z, u, v; //3d and texture coordinates

    int triLoop; //Triangle loop variable
    int verLoop; //Vertex loop variable

    readstr(filein, oneLine); //Grab a line from 'filein'
    sscanf(oneLine, "NUMPOLLIES %d\n", &numTriangles); //Read in number of triangle

    //Allocate space for triangles
    sector1.triangle = (triangle*)malloc(numTriangles*sizeof(triangle));
    sector1.numTriangles = numTriangles;

    //Get coords for each triangle
    for(triLoop = 0; triLoop < numTriangles; triLoop++ ){
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

//Read In A String
void readstr(FILE *f, char *string){
    do fgets( string, 255, f ); //Read One Line
    while((string[0] == '/')||(string[0] == '\n'));
    return;
}

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

void drawWorld(SDL_Window *window){
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
    glBindTexture(GL_TEXTURE_2D, texture[1]); //Select A Texture Based On filter

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
    SDL_GL_SwapWindow (window); //Draw it to the screen
}

//Function to release/destroy our resources and restoring the old desktop
void quitWorld(){
    SDL_Quit(); //Clean Up the wWndow
    if(sector1.triangle) free(sector1.triangle); //Deallocates allocated space
    exit(0); //Exit
}
