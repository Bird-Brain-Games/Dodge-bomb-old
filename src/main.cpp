// Core Libraries
#include <iostream>
#include <string>
#include <math.h>

// 3rd Party Libraries

#include "gl\glew.h"
#include "gl\freeglut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

//Project files
#include "objLoader.h"
#include "shaderLoader.h"
#include "shaders.h"

// Defines and Core variables
#define FRAMES_PER_SECOND 60
const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND; // Miliseconds per frame

int windowWidth = 800;
int windowHeight = 600;

int mousepositionX;
int mousepositionY;


// A few conversions to know
float degToRad = 3.14159f / 180.0f;
float radToDeg = 180.0f / 3.14159f;


/* function DisplayCallbackFunction(void)
* Description:
*  - this is the openGL display routine
*  - this draws the sprites appropriately
*/

float g = 0;
float r = 0.2;
float b = 0.2;


UINT uiVBO[2]; // One VBO for vertices positions, one for colors
UINT uiVAO[1]; // One VAO for pyramid

float rotation = 0;
UINT testing;

ShaderLoader vertShader;
ShaderLoader fragShader;
Loader object(3);

CShader shVertex, shFragment;
CShaderProgram spMain;


float fPyramid[36]; // Pyramid data - 4 triangles of 3 vertices of 3 floats
float fPyramidColor[36]; // Same for color

void initScene()
{
	object.load("pyramid");

	glGenVertexArrays(1, uiVAO);
	glGenBuffers(2, uiVBO);
	glBindVertexArray(uiVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, object.getVertexOrder().size() * sizeof(float), object.getVertexOrder().data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, object.getColorOrder().size() * sizeof(float), object.getColorOrder().data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);



	shVertex.loadShader("shaders\\shader.vert", GL_VERTEX_SHADER);
	shFragment.loadShader("shaders\\shader.frag", GL_FRAGMENT_SHADER);

	spMain.createProgram();
	spMain.addShaderToProgram(&shVertex);
	spMain.addShaderToProgram(&shFragment);

	spMain.linkProgram();
	spMain.useProgram();


	//vertShader.loadShader("shaders\\shader.vert", GL_VERTEX_SHADER);
	//fragShader.loadShader("shaders\\shader.frag", GL_FRAGMENT_SHADER);


	//testing = glCreateProgram();

	//glAttachShader(testing, vertShader.getID());
	//glAttachShader(testing, fragShader.getID());

	//glLinkProgram(testing);

	//GLint isLinked = 0;
	//glLinkProgram(testing);
	//glGetProgramiv(testing, GL_LINK_STATUS, (int *)&isLinked);
	//glUseProgram(testing);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

float tx = 0.0f;
float ty = 0.0f;
float tz = 0.0f;

float rx = 0.0f;
float ry = 0.0f;
float rz = 0.0f;

float sx = 1.0f;
float sy = 1.0f;
float sz = 1.0f;
int keydown[256];

void DisplayCallbackFunction(void)
{
	glLoadIdentity();
	rotation += 1;
	
	/* clear the screen */

//	glClearColor(0.0f, 0.2f, 0.3f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(uiVAO[0]);


	int iModelViewLoc = glGetUniformLocation(spMain.getProgramID(), "modelViewMatrix");
	//int iProjectionLoc = glGetUniformLocation(spMain.getProgramID(), "projectionMatrix");
	//glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(glm::perspective(90.0f, (float)windowWidth / windowHeight, 0.001f, 10000.0f))); // sends data to shader
	// first value is the variable being sent, second is the number of matrices, third is for transposing (glm and glsl use same), fourth is the location of the data being sent 
	//glm::mat4 mModelView = glm::lookAt(glm::vec3(0, 15, 40), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 mCurrent = glm::rotate(mModelView, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	// Render rotating pyramid in the middle
	//glm::vec3 test = glm::vec3(0.0, float(sin(rotation*degToRad)), 0.0f);
	//mCurrent = glm::translate(mModelView, test);
	//glm::mat4x4 mCurrent = { 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0 };
	//glm::mat4x4 rotationX = { sx, 0.0, 0.0, 0.0, 0.0, sy, 0.0, 0.0, 0.0, 0.0, sz, 0.0, tx, ty, tz, 1.0 };;
	//glm::mat4x4 rotationY;
	//glm::mat4x4 rotationZ;

	glm::mat4x4 mCurrent = { sx, 0.0, 0.0, 0.0, 0.0, sy, 0.0, 0.0, 0.0, 0.0, sz, 0.0, tx, ty, tz, 1.0 };
	glm::mat4x4 mCurrent2 = { sx, 0.0, 0.0, tx, 0.0, sy, 0.0, ty, 0.0, 0.0, sz, tz, 0.0, 0.0, 0.0, 1.0 };
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, object.getVertexOrder().size());



	//int iModelViewLoc = glGetUniformLocation(spMain.getProgramID(), "modelViewMatrix");
	//int iProjectionLoc = glGetUniformLocation(spMain.getProgramID(), "projectionMatrix");
	//glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(glm::perspective(90.0f, (float)windowWidth / windowHeight, 0.001f, 10000.0f))); // sends data to shader
	//// first value is the variable being sent, second is the number of matrices, third is for transposing (glm and glsl use same), fourth is the location of the data being sent 
	//glm::mat4 mModelView = glm::lookAt(glm::vec3(0, 15, 40), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 mCurrent = glm::rotate(mModelView, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	//// Render rotating pyramid in the middle
	//glm::vec3 test = glm::vec3(0.0, float(sin(rotation*degToRad)), 0.0f);
	//mCurrent = glm::translate(mModelView, test);
	//glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	//glDrawArrays(GL_TRIANGLES, 0, object.getVertexOrder().size());


	/* Swap Buffers to Make it show up on screen */
	glutSwapBuffers();

	//glDetachShader(testing, vertShader.getID());
	//glDetachShader(testing, fragShader.getID());

}

/* function void KeyboardCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is pressed
*/
void KeyboardCallbackFunction(unsigned char key, int x, int y)
{
	std::cout << "Key Down:" << (int)key << std::endl;

	keydown[key] = true;

}

/* function void KeyboardUpCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is lifted
*/
void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{
	keydown[key] = false;
}

/* function TimerCallbackFunction(int value)
* Description:
*  - this is called many times per second
*  - this enables you to animate things
*  - no drawing, just changing the state
*  - changes the frame number and calls for a redisplay
*  - FRAME_DELAY is the number of milliseconds to wait before calling the timer again
*/
void TimerCallbackFunction(int value)
{
	/* this call makes it actually show up on screen */
	glutPostRedisplay();
	/* this call gives it a proper frame delay to hit our target FPS */
	if (keydown['d'])
	{
		tx += 0.05;
	}
	if (keydown['a'])
	{
		tx -= 0.05;
	}
	if (keydown['q'])
	{
		tz += 0.05;
	}
	if (keydown['e'])
	{
		tz -= 0.05;
	}
	if (keydown['w'])
	{
		ty += 0.05;
	}
	if (keydown['s'])
	{
		ty -= 0.05;
	}


	if (keydown['r'])
	{
		sx += 0.05;
	}
	if (keydown['y'])
	{
		sx -= 0.05;
	}
	if (keydown['f'])
	{
		sy += 0.05;
	}
	if (keydown['h'])
	{
		sy -= 0.05;
	}
	if (keydown['v'])
	{
		sz += 0.05;
	}
	if (keydown['n'])
	{
		sz -= 0.05;
	}
	glutTimerFunc(FRAME_DELAY, TimerCallbackFunction, 0); // after x Ticks call again.
}

/* function WindowReshapeCallbackFunction()
* Description:
*  - this is called whenever the window is resized
*  - and sets up the projection matrix properly
*  - currently set up for an orthographic view (2D only)
*/
void WindowReshapeCallbackFunction(int w, int h)
{
	// switch to projection because we're changing projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, (float)w / h, 0.1f, 10000.0f);
	windowWidth = w;
	windowHeight = h;
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void MouseClickCallbackFunction(int button, int state, int x, int y)
{
	// Handle mouse clicks
	if (state == GLUT_DOWN)
	{
		//std::cout << button << std::endl;
		//std::cout << "Mouse X:" << x << " Mouse y: " << y << std::endl;
		//if (button == 0)
		//	std::cout << "you're a butt" << std::endl;

		//if (button == 1)
		//	std::cout << "da middle" << std::endl;

		//if (button == 2)
		//	std::cout << "da left" << std::endl;
	}



}


/* function MouseMotionCallbackFunction()
* Description:
*   - this is called when the mouse is clicked and moves
*/
void MouseMotionCallbackFunction(int x, int y)
{
}

/* function MousePassiveMotionCallbackFunction()
* Description:
*   - this is called when the mouse is moved in the window
*/
void MousePassiveMotionCallbackFunction(int x, int y)
{
	mousepositionX = x;
	mousepositionY = y;
}

/* function main()
* Description:
*  - this is the main function
*  - does initialization and then calls glutMainLoop() to start the event handler
*/

int main(int argc, char **argv)
{
	/* initialize the window and OpenGL properly */
	glutInit(&argc, argv);


	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("window title");//

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));




	initScene();
	/* set up our function callbacks */
	//alternative names

	glutDisplayFunc(DisplayCallbackFunction);//draw()
	glutKeyboardFunc(KeyboardCallbackFunction);//KeyDown()
	glutKeyboardUpFunc(KeyboardUpCallbackFunction);//KeyUp()
	glutReshapeFunc(WindowReshapeCallbackFunction); // windowResize()
	glutMouseFunc(MouseClickCallbackFunction);// mouseclick
	glutMotionFunc(MouseMotionCallbackFunction);//mouseMovedActive() clicking and moving etc
	glutPassiveMotionFunc(MousePassiveMotionCallbackFunction);//mouseMovedPassive() just mouse movement
	glutTimerFunc(1, TimerCallbackFunction, 0);//Timmer/clockTick function



	/* start the event handler */
	glutMainLoop();
	return 0;


}