//Create World
#include "World.h"


///////////////////////////////////////////////////////////
//                   GLOBAL VARIABLES                    //
SDL_Surface **TI;                                        //
GLuint texture[3]; //Storage for 3 Textures              //
GLuint filter; //Which Filter To Use                     //
                                                         //
sector sector1; //Our Sector                             //
///////////////////////////////////////////////////////////


//Main Function, Creates World
void genWorld(){
    initWorld(); //General OpenGL Initialization Function
    setupWorld("data/world.txt"); //Text File Holding World Layout
}

void initWorld(){
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
    
    glColor4f( 1.0f, 1.0f, 1.0f, 0.5f); //Full Brightness, 50% Alpha
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Blending Function For Translucency Based On Source Alpha Value
}

//Function to load in bitmap as a GL texture
void LoadGLTextures(){
    TI = new SDL_Surface *[3];
    
    TI[0] = IMG_Load("data/textures/floor/Floor4.jpg"); //Floor Texture
    TI[1] = IMG_Load("data/textures/roof/Roof5.jpg"); //Roof Texture
    TI[2] = IMG_Load("data/textures/wall/Wall1.jpg"); //Wall Texture

    //TEXTURE 1 (Floor Texture)
    glGenTextures(3, &texture[0]); //Create The Texture
    glBindTexture(GL_TEXTURE_2D, texture[0]); //Load in texture 1
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TI[0]->w, TI[0]->h, 0, FORMAT, GL_UNSIGNED_BYTE, TI[0]->pixels); //Generate The Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(TI[0]);
    
    //TEXTURE 2 (Roof Texture)
    glGenTextures(3, &texture[1]); //Create The Texture
    glBindTexture(GL_TEXTURE_2D, texture[1]); //Load in texture 2
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TI[1]->w, TI[1]->h, 0, FORMAT, GL_UNSIGNED_BYTE, TI[1]->pixels); //Generate The Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(TI[1]);
    
    //TEXTURE 3 (Wall Texture)
    glGenTextures(3, &texture[2]); //Create The Texture
    glBindTexture(GL_TEXTURE_2D, texture[2]); //Load in texture 3
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TI[2]->w, TI[2]->h, 0, FORMAT, GL_UNSIGNED_BYTE, TI[2]->pixels); //Generate The Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(TI[2]);
}

//Setup World
void setupWorld(string worldFile){
    FILE *filein = fopen(worldFile.c_str(), "rt"); //File To Work With

    int numTriangles; //Number of Triangles
    char oneLine[255]; //One line from conf file

    float x, y, z, u, v; //3d and texture coordinates
    int t = 0.0; //Texture Index

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
            
            if(verLoop == 0){ //First row of matrix holds an additional int. Handle here
                sscanf(oneLine, "%f %f %f %f %f %d\n", &x, &y, &z, &u, &v, &t);
                sector1.triangle[triLoop].vertex[verLoop].t = t; //Texture Index
            }
		    else sscanf(oneLine, "%f %f %f %f %f\n", &x, &y, &z, &u, &v);
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

void drawWorld(SDL_Window *window){
    GLfloat x_m, y_m, z_m, u_m, v_m; //Floating Point For Temp X, Y, Z, U And V Vertices
    int t_m;

    int loop_m; //Loop variable
    
    //Process Each Triangle
    for(loop_m = 0; loop_m < sector1.numTriangles; loop_m++){
        t_m = sector1.triangle[loop_m].vertex[0].t;
        glBindTexture(GL_TEXTURE_2D, texture[t_m]); //Select A Texture Based On Texture Code
        
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
