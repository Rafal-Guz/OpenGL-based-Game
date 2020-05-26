// Includes
#include <GL/glew.h>
#include <GL/glut.h>
#include <Shader.h>
#include <Vector.h>
#include <Matrix.h>
#include <Mesh.h>
#include <Texture.h>
#include <SphericalCameraManipulator.h>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

struct Torus{
	bool valid;
	float X;
	float Y;
	float Z;
	float angle;
};

struct pair{
	int first;
	int second;
};

// Function Prototypes
// Provided ones ------------------------------------------------------------//
bool initGL(int argc, char** argv);
void display(void);
void keyboard(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void handleKeys();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
// Added ones ---------------------------------------------------------------//
void initShader();
void initGeometry();
void drawGeometry();
void initTexture(std::string filename, GLuint & textureID);
void initTorus();
void drawTorus();
void drawTerrain();
void drawPlane();
void hud();
void checkCollision();
void menu();
void difficulty_menu();
void control_menu();
void game_over_menu();
void render2dText(std::string text, float r,float g,float b, float x,float y);

// Screen size
int screenWidth   	        = 1000;
int screenHeight   	        = 1000;

// Array of key states
bool keyStates[256];

//Global Variables
GLuint shaderProgramID;			                    	// Shader Program ID
GLuint vertexPositionAttribute;		                // Vertex Position Attribute Location
GLuint vertexNormalAttribute;
GLuint vertexTexcoordAttribute;		                // Vertex Texcoord Attribute
Vector3f trans = Vector3f(0,0,0);									//	Translate Location
float angle_d = 0.f;
float angle_r;

//Material Properties
GLuint LightPositionUniformLocation;                // Light Position Uniform
GLuint AmbientUniformLocation;
GLuint SpecularUniformLocation;
GLuint SpecularPowerUniformLocation;
GLuint ColourUniformLocation;

Vector3f lightPosition= Vector3f(0.0,0.0,10.0);   // Light Position
Vector3f ambient    = Vector3f(0.15,0.15,0.15);
Vector3f specular   = Vector3f(1.0,1.0,1.0);
Vector3f colour = Vector3f(1.0,1.0,1.0);
float specularPower = 20.0;
bool game_start = false;
bool resume = false;
bool diff_menu = false;
bool ctrl_menu = false;
bool game_over = false;
bool torus_set = false;
bool game_won = false;
bool free_flight = false;
bool game_sound = false;
bool game_sound2 = false;
int points = 0;
float velocity_max = 0.2;
float velocity_min = 0.05;//0.05;
float velocity = velocity_min;
float angle_x = 0.f;
float angle_z = 0.f;

GLuint TextureMapUniformLocation;           // Texture Map Location
GLuint texture_p;				                    	// OpenGL Texture
GLuint texture_t;
GLuint texture_g;

//Viewing
SphericalCameraManipulator cameraManip;
Matrix4x4 ModelViewMatrix;		                    // ModelView Matrix
GLuint MVMatrixUniformLocation;		                // ModelView Matrix Uniform
Matrix4x4 ProjectionMatrix;		                    // Projection Matrix
GLuint ProjectionUniformLocation;	                // Projection Matrix Uniform Location
Matrix4x4 TempMatrix;
pair
hard_matrix[18][18] = {
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{-1,50},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,35},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{4,30},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{-3,25},{0,0},{0,0},{0,0},{0,0},{0,0},{6,10},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{2,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{-2,45},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,40},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{2,60},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{3,90},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}} };

pair
easy_matrix[18][18] = {	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{-1,50},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,35},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{4,30},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{-2,45},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{-3,25},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{2,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{6,10},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,40},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{2,60},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
												{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{3,90},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}} };

//Mesh
Mesh plane;
Mesh torus;
Mesh grass;
Mesh sidea;
Mesh sideb;
Mesh sidec;
Mesh sided;
Mesh trab;
Mesh trbc;
Mesh trcd;
Mesh trda;


float slope = 0.84f;
float radius = 5.f;
float game_time = 0;
int total_rings = 10;
int rings = total_rings;
std::string hard = "HARD";
std::string easy = "EASY";
std::string difficulty = easy;
Torus* list = new Torus [total_rings];
float hard_inner = 1*2.5;
float hard_outer = 0.25*2.5;
float easy_inner = 1*3;
float easy_outer = 0.25*3;
float e_scale = 3;
float h_scale = 2.5;
float scale = e_scale;
Mix_Chunk *main_music;
Mix_Chunk *background_music;


//Main Program Entry
int main(int argc, char** argv)
{
	//init OpenGL
	if(!initGL(argc, argv))
		return -1;

	//Init SDL audio systems
	SDL_Init(SDL_INIT_AUDIO);
	//Init SDL mixer (SampleRate, AudioFormat, Channels, BufferSize)
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512);

	//Init Key States to false;
	for(int i(0) ; i < 256; i++)
			keyStates[i] = false;

  //Init OpenGL Shader
  initShader();

	//Init Mesh Geometry
	plane.loadOBJ("../models/plane1.obj");
	torus.loadOBJ("../models/torus.obj");
	grass.loadOBJ("../models/grass.obj");
	sidea.loadOBJ("../models/sidea.obj");
	sideb.loadOBJ("../models/sideb.obj");
	sidec.loadOBJ("../models/sidec.obj");
	sided.loadOBJ("../models/sided.obj");
	trab.loadOBJ("../models/trab.obj");
	trbc.loadOBJ("../models/trbc.obj");
	trcd.loadOBJ("../models/trcd.obj");
	trda.loadOBJ("../models/trda.obj");

	//Init audio files
	background_music = Mix_LoadWAV("../common/mus01.wav");
	main_music = Mix_LoadWAV("../common/mus02.wav");

	//Init textures
	initTexture("../models/plane1.bmp", texture_p);
  initTexture("../models/Crate.bmp", texture_t);
	initTexture("../models/grass.bmp", texture_g);

  //Init Camera Manipultor
	cameraManip.setPanTiltRadius(0.f,-slope,radius);

	//Start main loop
	glutMainLoop();

	//Clean-Up
	glDeleteProgram(shaderProgramID);
	Mix_FreeChunk(background_music);
	Mix_FreeChunk(main_music);
  Mix_CloseAudio();
	SDL_Quit();

  return 0;
}

//! Function to Initlise OpenGL
bool initGL(int argc, char** argv)
{
	//Init GLUT
  glutInit(&argc, argv);

	//Set Display Mode: single buffered
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	//Set Window Size
  glutInitWindowSize(screenWidth, screenHeight);

  // Window Position
  glutInitWindowPosition(100, 100);

	//Create Window
  glutCreateWindow("Flight Simulator");

  // Init GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	//Set Display function
  glutDisplayFunc(display);

	//Set Keyboard Interaction Functions
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyUp);

	//Set Mouse Interaction Functions
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
	return true;
}

//Init Shader
void initShader()
{
	//Create shader
  shaderProgramID = Shader::LoadFromFile("shader.vert","shader.frag");

  // Get a handle for our vertex position buffer
	vertexPositionAttribute = glGetAttribLocation(shaderProgramID,  "aVertexPosition");
	vertexNormalAttribute = glGetAttribLocation(shaderProgramID,    "aVertexNormal");
	vertexTexcoordAttribute = glGetAttribLocation(shaderProgramID, "aVertexTexcoord");

  //!
	MVMatrixUniformLocation         = glGetUniformLocation(shaderProgramID, "MVMatrix_uniform");
	ProjectionUniformLocation       = glGetUniformLocation(shaderProgramID, "ProjMatrix_uniform");
	LightPositionUniformLocation    = glGetUniformLocation(shaderProgramID, "LightPosition_uniform");
	ColourUniformLocation           = glGetUniformLocation(shaderProgramID, "Colour_uniform");
	AmbientUniformLocation          = glGetUniformLocation(shaderProgramID, "Ambient_uniform");
	SpecularUniformLocation         = glGetUniformLocation(shaderProgramID, "Specular_uniform");
	SpecularPowerUniformLocation    = glGetUniformLocation(shaderProgramID, "SpecularPower_uniform");
  TextureMapUniformLocation       = glGetUniformLocation(shaderProgramID, "TextureMap_uniform");
}

void initTexture(std::string filename, GLuint & textureID)
{
	//Generate texture and bind
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Get texture Data
	int width, height;
	char* data;
	Texture::LoadBMP(filename, width, height, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

  //Cleanup data - copied to GPU
  delete[] data;
}

void initTorus()
{
	int z(0),x(0);
	if(difficulty == easy)
	{
		scale = e_scale;
		for(int i(0); i<total_rings; i++)	// Loop through rings
		{
			while(easy_matrix[z][x].first == 0)	// Empty
			{
				x++;
				if(x>17)
				{
					x=0;
					z++;
				}
			}
			list[i].valid = true;
			list[i].X = (x-9)*5;
			list[i].Y = easy_matrix[z][x].first;
			list[i].Z = (z-9)*5;
			list[i].angle = easy_matrix[z][x].second;
			x++;
			if(x>17)
			{
				x=0;
				z++;
			}
		}
	}
	else
	{
		scale = h_scale;
		for(int i(0); i<total_rings; i++)	// Loop through rings
		{
			while(easy_matrix[z][x].first == 0)	// Empty
			{
				x++;
				if(x>17)
				{
					x=0;
					z++;
				}
			}
			list[i].valid = true;
			list[i].X = (x-9)*5;
			list[i].Y = hard_matrix[z][x].first;
			list[i].Z = (z-9)*5;
			list[i].angle = hard_matrix[z][x].second;
			x++;
			if(x>17)
			{
				x=0;
				z++;
			}
		}
	}
	torus_set = true;
}

//Display Loop
void display(void)
{
	//Handle keys
	handleKeys();

	//Set Viewport
	glViewport(0,0,screenWidth, screenHeight);

	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

if (!game_start)	// Not actul game
{
	if (!game_sound)
	{
		Mix_PlayChannel(0, background_music, -1);
		game_sound = true;
		game_sound2 = false;
	}
	if(diff_menu)
		difficulty_menu();
	else if(ctrl_menu)
		control_menu();
	else if(game_over)
		game_over_menu();
	else
		menu();
}
else
{
	if (!game_sound2)
	{
		Mix_PlayChannel(0, main_music, -1);
		game_sound2 = true;
		game_sound = false;
	}
	// Drawing the whole scene

	// Background
	glClearColor(0.2,0.8,1.0,1.0);
  //Use shader
	glUseProgram(shaderProgramID);

  //Projection Matrix - Perspective Projection
  ProjectionMatrix.perspective(90, 1.0, 0.0001, 100.0);

  //Set Projection Matrix
  glUniformMatrix4fv(
	ProjectionUniformLocation,  //Uniform location
	1,							//Number of Uniforms
	false,					//Transpose Matrix
	ProjectionMatrix.getPtr());	//Pointer to ModelViewMatrixValues

	cameraManip.setFocus(trans);

	ModelViewMatrix.toIdentity();
	ModelViewMatrix = cameraManip.apply(ModelViewMatrix);

	drawTorus();

	drawTerrain();

	drawPlane();

	glUniform3f(ColourUniformLocation, colour.x,colour.y,colour.z);
	glUniform3f(LightPositionUniformLocation, lightPosition.x,lightPosition.y,lightPosition.z);
  glUniform4f(AmbientUniformLocation, ambient.x, ambient.y, ambient.z, 1.0);
  glUniform4f(SpecularUniformLocation, specular.x, specular.y, specular.z, 1.0);
  glUniform1f(SpecularPowerUniformLocation, specularPower);

	//Unuse Shader
	glUseProgram(0);

	trans=trans+Vector3f(velocity*sin(angle_r), 0, velocity*cos(angle_r));
	hud();
	checkCollision();
	cameraManip.setPanTiltRadius(angle_r,-slope,radius);
}
	//Swap Buffers and post redisplay
	glutSwapBuffers();
	glutPostRedisplay();
}

void drawTorus()
{
	// Set texture
	glBindTexture(GL_TEXTURE_2D, texture_t);
	for(int i(0); i < total_rings; i++)
	{
		if(list[i].valid)
		{
			// Change place
			TempMatrix = ModelViewMatrix;
			TempMatrix.translate(list[i].X, list[i].Y, list[i].Z);
			//TempMatrix.rotate(list[i].angle, 0, 1, 0);
			TempMatrix.rotate(90, 1, 0, 0);
			TempMatrix.scale(scale,scale,scale);
			glUniformMatrix4fv(
			MVMatrixUniformLocation,  	//Uniform location
			1,					        //Number of Uniforms
			false,				        //Transpose Matrix
			TempMatrix.getPtr());	        //Pointer to Matrix Values
			torus.Draw(vertexPositionAttribute, vertexNormalAttribute,vertexTexcoordAttribute);
		}
	}
}

void drawTerrain()
{
	// Set texture
	glBindTexture(GL_TEXTURE_2D, texture_g);

	// Set position
	glUniformMatrix4fv(
	MVMatrixUniformLocation,  	//Uniform location
	1,					        //Number of Uniforms
	false,				        //Transpose Matrix
 	ModelViewMatrix.getPtr());	        //Pointer to Matrix Values

	// Draw
	grass.Draw(vertexPositionAttribute, vertexNormalAttribute,vertexTexcoordAttribute);
	sidea.Draw(vertexPositionAttribute, vertexNormalAttribute,vertexTexcoordAttribute);
	sideb.Draw(vertexPositionAttribute, vertexNormalAttribute,vertexTexcoordAttribute);
	sidec.Draw(vertexPositionAttribute, vertexNormalAttribute,vertexTexcoordAttribute);
	sided.Draw(vertexPositionAttribute, vertexNormalAttribute,vertexTexcoordAttribute);
	trab.Draw(vertexPositionAttribute, vertexNormalAttribute,vertexTexcoordAttribute);
	trbc.Draw(vertexPositionAttribute, vertexNormalAttribute,vertexTexcoordAttribute);
	trcd.Draw(vertexPositionAttribute, vertexNormalAttribute,vertexTexcoordAttribute);
	trda.Draw(vertexPositionAttribute, vertexNormalAttribute,vertexTexcoordAttribute);
}

void drawPlane()
{
  // Set texture
	glBindTexture(GL_TEXTURE_2D, texture_p);

	// Change place
	TempMatrix = ModelViewMatrix;
	TempMatrix.translate(trans.x, trans.y, trans.z);
	TempMatrix.rotate(angle_d, 0, 1, 0);	// Y axes
	TempMatrix.rotate(angle_x, 1, 0, 0);	// X axes
	TempMatrix.rotate(angle_z, 0, 0, 1);	// Z axes

	glUniformMatrix4fv(
	MVMatrixUniformLocation,  	//Uniform location
	1,					        //Number of Uniforms
	false,				        //Transpose Matrix
  TempMatrix.getPtr());	        //Pointer to Matrix Values

	// Draw
	plane.Draw(vertexPositionAttribute, vertexNormalAttribute,vertexTexcoordAttribute);
}

void hud()
{
	if(!free_flight)
		game_time += 1.0/60.0;
	std::stringstream time_stream, ring_stream, point_stream, vel_stream;
	std::string time_str, ring_str, point_str, vel_str;
	time_stream.precision(3);
	time_stream << game_time;
	time_stream >> time_str;
	render2dText("Game Time: ", 0.1, 0.6, 0.1, -0.9, 0.9);
	render2dText(time_str, 0.0, 0.0, 0.0, -0.63, 0.9);
	ring_stream << rings;
	ring_stream >> ring_str;
	render2dText("Remaining Rings: ", 0.5, 0.0, 0.0, -0.5, 0.9);
	render2dText(ring_str, 0.0, 0.0, 0.0, -0.13, 0.9);
	point_stream << points;
	point_stream >> point_str;
	render2dText("Points: ", 0.7, 0.7, 0.0, -0.05, 0.9);
	render2dText(point_str, 0.0, 0.0, 0.0, 0.1, 0.9);
	vel_stream << velocity*10;
	vel_stream.precision(3);
	vel_stream >> vel_str;
	render2dText("Velocity: ", 0.7, 0.7, 1.0, 0.5, -0.8);
	render2dText(vel_str, 1.0, 1.0, 1.0, 0.7, -0.8);
	render2dText("Ma", 0.7, 0.7, 1.0, 0.8, -0.8);
}

void checkCollision()
{
	if(difficulty == easy)	// Not that strict rules - can a bit go through
	{
		// Loops over all torusses
		for(int i(0); i < total_rings; i++)
		{
			if(list[i].valid)
			{
				if( abs(trans.z - list[i].Z) < 0.2 )	// Diff between plane and ring
				{
					float rad = sqrt(pow(trans.x - list[i].X,2) + pow(trans.y - list[i].Y,2));

					if(rad < easy_inner + easy_outer)	// Smaller + bigger
					{
						if(rad > easy_inner - easy_outer)	// Game over - inside ring
						{
							game_over = true;
							game_start = false;
							std::cout << "Lost due to collision" << std::endl;
						}
						else	// Got point
						{
							list[i].valid = false;
							rings--;
							// Add points
							if(rad < (easy_inner - easy_outer)/3.0)
								points += 8;
							else if(rad < 2*(easy_inner - easy_outer)/3.0)
								points += 6;
							else
								points += 4;
						}
					}
				}
			}
		}
	}
	else	// Really strict rules - cannot go through
	{
		// Loops over all torusses
		for(int i(0); i < total_rings; i++)
		{
			if(list[i].valid)
			{
				if( abs(trans.z - list[i].Z) < 0.2 )	// Diff between plane and ring
				{
					float rad = sqrt(pow(trans.x - list[i].X,2) + pow(trans.y - list[i].Y,2));
					// In range for calculations
					if(rad < hard_inner + hard_outer + 1)
					{
						// Can touch ring by the wing
						if(rad > hard_inner - hard_outer - 1)	// Game over
						{
							game_over = true;
							game_start = false;
							std::cout << "Lost due to collision" << std::endl;
						}
						else	// Got point
						{
							list[i].valid = false;
							rings--;
							// Add points
							if(rad < (hard_inner - hard_outer)/3.0)
								points += 8;
							else if(rad < 2*(hard_inner - hard_outer)/3.0)
								points += 4;
							else
								points += 2;
						}
					}
				}
			}
		}
	}
	if((rings == 0) && (!free_flight))	// You have won!
	{
		game_won = true;
		game_over = true;
		game_start = false;
	}
	if((abs(trans.x) > 45) || (abs(trans.z) > 45) || (abs(trans.y) > 5))	// Be careful - barrier
	{
		render2dText("I have bad feeling about this - turn back!", 0.9, 0.1, 0.1, -0.4, 0.75);

		if((abs(trans.x) > 65) || (abs(trans.z) > 65) || (abs(trans.y) > 9))	// Game over
		{
			game_won = false;
			game_start = false;
			game_over = true;
			std::cout << "Lost due to desertion" << std::endl;
		}
	}
}

// Keyboard Interaction
void keyboard(unsigned char key, int x, int y)
{
	// Quits to menu or resume the game
	if (key == 27)	// Esc key code
	{
		if(game_start)
		{
			game_start = false;
			resume = true;
		}
		else if(resume)
		{
			game_start = !game_start;
		}
	}

	//Set key status
	keyStates[key] = true;
}

// Handle key up situation
void keyUp(unsigned char key, int x, int y)
{
    keyStates[key] = false;
}

// Handle Keys
void handleKeys()
{
	bool checked_x = false;
	bool checked_z = false;
	float increase = 0.7;
	float decrease = 0.6;
	float angle_max = 45;
	// In the game
	if (game_start)
	{
		// Movements
		if(keyStates['w'])	// Forward
		{
			velocity += 0.001;
			if(velocity > velocity_max)
				velocity = velocity_max;
		}
		if(keyStates['s'])	// Backwards
		{
			velocity -= 0.001;
			if(velocity < velocity_min)
				velocity = velocity_min;
		}
		if(keyStates['a']) // Left
		{
			angle_d += 0.5;
			angle_z -= increase;
			if(angle_z < -angle_max)
				angle_z = angle_max;
			angle_r = angle_d * M_PI / 180.f;
		}
		else
		{
			if(angle_z > decrease)
				angle_z -= decrease;
			if(angle_z < -decrease)
				angle_z += decrease;
			checked_z = true;
		}
		if(keyStates['d'])	// Right
		{
			angle_d -= 0.5;
			angle_z += increase;
			if(angle_z > angle_max)
				angle_z = angle_max;
			angle_r = angle_d * M_PI / 180.f;
		}
		else
		{
			if(!checked_z)
			{
				if(angle_z > decrease)
					angle_z -= decrease;
				if(angle_z < -decrease)
					angle_z += decrease;
			}
		}
		if(keyStates['q'])	// Down
		{
			angle_x += increase;
			if(angle_x > angle_max)
				angle_x = angle_max;
			trans=trans-Vector3f(0,velocity/3.0,0);
		}
		else
		{
			if(angle_x > decrease)
				angle_x -= decrease;
			if(angle_x < -decrease)
				angle_x += decrease;
			checked_x = true;
		}
		if(keyStates['e'])	// Up
		{
			angle_x -= increase;
			if(angle_x < -angle_max)
				angle_x = angle_max;
			trans=trans+Vector3f(0,velocity/3.0,0);
		}
		else
		{
			if(!checked_x)
			{
				if(angle_x > decrease)
					angle_x -= decrease;
				if(angle_x < -decrease)
					angle_x += decrease;
			}
		}
		/*if(keyStates['l'])
		{
			std::cout << "Angle :   " << angle_d << " in degrees" << std::endl;
			std::cout << "Angle :   " << angle_r << " in radians\n" << std::endl;
		}
		if(keyStates['p']) // Print model-view matrix
		{
			ModelViewMatrix.print("MV");
		}*/
	}
}

// Mouse Interaction
void mouse(int button, int state, int x, int y)
{
	if (!game_start)	// Not gaming
	{
		if(diff_menu)	// Difficulty Menu
		{
			if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		  {
		    if(x < 400 && x > 350 && y < 525 && y > 475)	// EASY
				{
					if(!torus_set)
					{
						difficulty = easy;
						diff_menu = false;
					}
				}
				else if(x < 650 && x > 600 && y < 525 && y > 475)	// HARD
				{
					if(!torus_set)
					{
						difficulty = hard;
						diff_menu = false;
					}
				}
				else if(x < 525 && x > 475 && y < 775 && y > 725)	// Back
				{
					diff_menu = false;
				}
			}
		}
		else if(ctrl_menu)	// Controls Menu
		{
			if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		  {
				if(x < 525 && x > 475 && y < 775 && y > 725)	// Back
				{
					ctrl_menu = false;
				}
			}
		}
		else if(game_over)	// Game Over Menu
		{
			if(game_won)	// WON
			{
				if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			  {
					if(x < 600 && x > 550 && y < 775 && y > 725)	// Back
					{
						game_over = false;
						game_start = true;
						free_flight = true;
					}
					if(x < 450 && x > 400 && y < 775 && y > 725)	// Exit to menu
					{
						game_over = false;
						game_start = false;
						trans = Vector3f(0,0,0);
						for(int n(0); n<total_rings; n++)
							list[n].valid = true;
						points = 0;
						game_time = 0;
						rings = total_rings;
						torus_set = false;
						resume = false;
						velocity = velocity_min;
						angle_d = 0;
						angle_r = 0;
					}
				}
			}
			else	//LOST
			{
				if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			  {
					if(x < 525 && x > 475 && y < 775 && y > 725)	// Exit to menu
					{
						game_over = false;
						game_start = false;
						trans = Vector3f(0,0,0);
						for(int n(0); n<total_rings; n++)
							list[n].valid = true;
						points = 0;
						game_time = 0;
						rings = total_rings;
						torus_set = false;
						resume = false;
						velocity = velocity_min;
						angle_d = 0;
						angle_r = 0;
					}
				}
			}
		}
		else	// Main Menu
		{
			if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		  {
		    if(x < 600 && x > 400 && y < 475 && y > 425)	// Start
				{
					game_start = !game_start;
					resume = true;
					if(!torus_set)
						initTorus();
				}
				else if(x < 550 && x > 450 && y < 575 && y > 525)	// Difficulty
				{
					diff_menu = true;
				}
				else if(x < 550 && x > 450 && y < 675 && y > 625)	// Controls
				{
					ctrl_menu = true;
				}
				else if(x < 525 && x > 475 && y < 775 && y > 725)	// Exit
				{
					exit(0);
				}
		  }
		}
	}
  cameraManip.handleMouse(button, state,x,y);
  glutPostRedisplay();
}

// Motion
void motion(int x, int y)
{
	cameraManip.handleMouseMotion(x,y);
	glutPostRedisplay();
}

void menu()
{
	glClearColor(0.0,0.0,0.0,1.0);
	if (resume)
	{
		render2dText("Resume", 0.3, 0.8, 0.2, -0.075, 0.1);
		render2dText("PAUSED", 0.8, 0.8, 0.8, -0.8, 0.8);
	}
	else
	{
		if (difficulty == "EASY")
			render2dText("Start Easy Game", 0.3, 0.8, 0.2, -0.17, 0.1);
		else
			render2dText("Start Hard Game", 0.3, 0.8, 0.2, -0.17, 0.1);
	}

	render2dText("FLIGHT SIMULATOR", 1.0, 0.2, 0.8, -0.23, 0.4);
	render2dText("Difficulty", 1.0, 0.6, 0.0, -0.085, -0.1);
	render2dText("Controls", 0.6, 0.6, 1.0, -0.08, -0.3);
	render2dText("Exit", 0.3, 0.8, 0.6, -0.035, -0.5);
}

void difficulty_menu()
{
	render2dText("Current difficulty:", 1.0, 0.6, 0.0, -0.25, 0.3);

	if (difficulty == "EASY")
		render2dText(difficulty, 0.3, 1.0, 0.3, 0.15, 0.3);
	else
		render2dText(difficulty, 1.0, 0.3, 0.3, 0.15, 0.3);

	render2dText("EASY", 0.3, 1.0, 0.3, -0.3, 0.0);
	render2dText("HARD", 1.0, 0.3, 0.3, 0.2, 0.0);
	render2dText("Back", 0.3, 0.8, 0.6, -0.035, -0.5);
}

void control_menu()
{
	render2dText("Accelerate", 0.6, 0.6, 1.0, -0.5, 0.45);
	render2dText("W", 0.3, 0.8, 0.2, -0.22, 0.45);
	render2dText("Up", 0.6, 0.6, 1.0, -0.43, 0.2);
	render2dText("E", 0.3, 0.8, 0.2, -0.22, 0.2);
	render2dText("Left", 0.6, 0.6, 1.0, -0.44, -0.05);
	render2dText("A", 0.3, 0.8, 0.2, -0.22, -0.05);

	render2dText("Decelerate", 0.6, 0.6, 1.0, 0.1, 0.45);
	render2dText("S", 0.3, 0.8, 0.2, 0.39, 0.45);
	render2dText("Down", 0.6, 0.6, 1.0, 0.14, 0.2);
	render2dText("Q", 0.3, 0.8, 0.2, 0.39, 0.2);
	render2dText("Right", 0.6, 0.6, 1.0, 0.15, -0.05);
	render2dText("D", 0.3, 0.8, 0.2, 0.39, -0.05);

	render2dText("Pause/Resume", 0.6, 0.6, 1.0, -0.27, -0.30);
	render2dText("Esc", 0.3, 0.8, 0.2, 0.12, -0.30);

	render2dText("Back", 0.3, 0.8, 0.4, -0.035, -0.5);
}

void game_over_menu()
{
	glClearColor(0.0,0.0,0.0,1.0);
	std::stringstream time_stream, ring_stream, point_stream;
	std::string time_str, ring_str, point_str;
	time_stream.precision(3);
	time_stream << game_time;
	time_stream >> time_str;
	ring_stream << rings;
	ring_stream >> ring_str;
	point_stream << points;
	point_stream >> point_str;

	if(game_won)
	{
		render2dText("You have WON!", 8.0, 0.0, 0.0, -0.15, 0.5);
		render2dText("Back", 0.3, 0.8, 0.6, 0.12, -0.5);
		render2dText("Exit", 0.3, 0.8, 0.6, -0.2, -0.5);
	}
	else
	{
		render2dText("G A M E  O V E R", 8.0, 0.0, 0.0, -0.18, 0.5);
		render2dText("Exit", 0.3, 0.8, 0.6, -0.035, -0.5);
	}

	render2dText("Difficulty:", 1.0, 0.6, 0.0, -0.25, 0.3);
	if (difficulty == "EASY")
		render2dText("EASY", 0.3, 0.8, 0.3, 0.12, 0.3);
	else
		render2dText("HARD", 0.8, 0.3, 0.3, 0.12, 0.3);

	render2dText("Game Time:", 0.1, 0.6, 0.1, -0.28, 0.1);
	render2dText(time_str, 0.5, 0.2, 0.7, 0.13, 0.1);

	render2dText("Rings left:", 0.5, 0.5, 1.0, -0.27, -0.1);
	render2dText(ring_str, 0.8, 0.3, 0.8, 0.165, -0.1);

	render2dText("Points:", 0.9, 0.9, 0.0, -0.23, -0.3);
	render2dText(point_str, 1.0, 0.7, 0.7, 0.155, -0.3);
}

void render2dText(std::string text, float r, float g, float b,
float x, float y)
{
	glColor3f(r,g,b);
	glRasterPos2f(x, y); // window coordinates
	for(unsigned int i = 0; i < text.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
}
