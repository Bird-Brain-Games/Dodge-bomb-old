// Core Libraries
#include <iostream>
#include <string>
#include <math.h>

// 3rd Party Libraries

#include "gl\glew.h"
#include "gl\freeglut.h"
#include "glm\glm.hpp"
//Project files
#include "objLoader.h"
#include "shaderLoader.h"

// Defines and Core variables
#define FRAMES_PER_SECOND 60
const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND; // Miliseconds per frame

int windowWidth = 800;
int windowHeight = 600;

int mousepositionX;
int mousepositionY;


// A few conversions to know



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

void DisplayCallbackFunction(void)
{

	/* clear the screen */
	glViewport(0, 0, windowWidth, windowHeight);


	Loader asdfa(3);
	asdfa.load("spongebob_bind");
	glClearColor(0.0f, 0.2f, 0.3f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//std::vector<float> vertexs;
	//std::vector<float> colours;
	//for (int count = 0; count <= asdfa.getOrder().size() - 1; count += 3)
	//{
	//	switch ((count / 3) % 3)
	//	{
	//	case 0:
	//		colours.push_back(0.5f);
	//		colours.push_back(0.0f);
	//		colours.push_back(0.0f);
	//		break;
	//	case 1:
	//		colours.push_back(0.0f);
	//		colours.push_back(0.5f);
	//		colours.push_back(0.0f);
	//		break;
	//	case 2:
	//		colours.push_back(0.0f);
	//		colours.push_back(0.0f);
	//		colours.push_back(0.5f);
	//		break;
	//	}
	//	int temp = ((asdfa.getOrder().at(count) - 1) * 3);
	//	vertexs.push_back(asdfa.getVertex().at(temp));
	//	vertexs.push_back(asdfa.getVertex().at(temp + 1));
	//	vertexs.push_back(asdfa.getVertex().at(temp + 2));
	//}

	//	glGenVertexArrays(1, uiVAO);
	//glGenBuffers(2, uiVBO);

	//// Setup whole pyramid
	//glBindVertexArray(uiVAO[0]);

	//glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	//glBufferData(GL_ARRAY_BUFFER, 36*sizeof(float), fPyramid, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	//glBufferData(GL_ARRAY_BUFFER, 36*sizeof(float), fPyramidColor, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBegin(GL_TRIANGLES);

	for (int count = 0; count <= asdfa.getOrder().size() - 1; count += 3)
	{
		switch ((count / 3) % 3)
		{
		case 0:
			glColor3f(0.5f, 0.0f, 0.0f);
			break;
		case 1:
			glColor3f(0.0f, 0.5f, 0.0f);
			break;
		case 2:
			glColor3f(0.0f, 0.0f, 0.5f);
			break;
		}
		int temp = ((asdfa.getOrder().at(count) - 1) * 3);
		glVertex3f(asdfa.getVertex().at(temp), asdfa.getVertex().at(temp + 1), asdfa.getVertex().at(temp + 2));
	}
	glEnd();


	/* This is where we draw things */


	//glColor3f(0.5f, 0.2f, 0.1f);
	//glBegin(GL_TRIANGLE_FAN);

	//glVertex3f(-0.25f, -0.25f, 0.0f);//-- bottom left
	//glVertex3f(0.25f, -0.25f, 0.0f);//+- bottom right
	//glVertex3f(0.0f, 0.0f, -0.25f);//++ top right

	//glEnd();
	//glColor3f(0.5f, 0.2f, 0.1f);
	//glBegin(GL_TRIANGLES);

	//for (float count = 0; count <= 2; count++)
	//{

	//	glVertex3f(count, 0.0f, 0.0f);//-- bottom left
	//}
	//glVertex3f(0.5f, 1.0f, 0.0f);//-- bottom left
	//glEnd();






	//fPyramid[0] = 0.0f; fPyramid[1] = 5.0f; fPyramid[2] = 0.0f;
	//fPyramid[3] = -3.0f; fPyramid[4] = 0.0f; fPyramid[5] = 3.0f;
	//fPyramid[6] = 3.0f; fPyramid[7] = 0.0f; fPyramid[8] = 3.0f;
	//// Back face
	//fPyramid[9] = 0.0f; fPyramid[10] = 5.0f; fPyramid[11] = 0.0f;
	//fPyramid[12] = 3.0f; fPyramid[13] = 0.0f; fPyramid[14] = -3.0f;
	//fPyramid[15] = -3.0f; fPyramid[16] = 0.0f; fPyramid[17] = -3.0f;
	// Left face
	//fPyramid[18] = 0.0f; fPyramid[19] = 5.0f; fPyramid[20] = 0.0f;
	//fPyramid[21] = -3.0f; fPyramid[22] = 0.0f; fPyramid[23] = -3.0f;
	//fPyramid[24] = -3.0f; fPyramid[25] = 0.0f; fPyramid[26] = 3.0f;

	//// Right face
	//fPyramid[27] = 0.0f; fPyramid[28] = 5.0f; fPyramid[29] = 0.0f;
	//fPyramid[30] = 3.0f; fPyramid[31] = 0.0f; fPyramid[32] = 3.0f;
	//fPyramid[33] = 3.0f; fPyramid[34] = 0.0f; fPyramid[35] = -3.0f;



	/* Swap Buffers to Make it show up on screen */
	glutSwapBuffers();

}

/* function void KeyboardCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is pressed
*/
void KeyboardCallbackFunction(unsigned char key, int x, int y)
{
	std::cout << "Key Down:" << (int)key << std::endl;

	switch (key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(0);
		break;
	}
}

/* function void KeyboardUpCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is lifted
*/
void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{
	glViewport(0, 0, windowWidth, windowHeight);



	glClearColor(0.0, 0.0, 0.0, 0.8f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();





	glutSwapBuffers();
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
	float asp = (float)w / (float)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	windowHeight = h;
	windowWidth = w;


	//switch back to modelview
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
GLuint testing;

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

	ShaderLoader vertShader;
	ShaderLoader fragShader;


	vertShader.loadShader("shaders\\shader.vert", GL_VERTEX_SHADER);
	fragShader.loadShader("shaders\\shader.frag", GL_FRAGMENT_SHADER);

	GLuint glCreateProgram​();

	testing = glCreateProgram();

	glAttachShader(testing, vertShader.getID());
	glAttachShader(testing, fragShader.getID());

	glLinkProgram(testing);

	GLint isLinked = 0;
	glGetProgramiv(testing, GL_LINK_STATUS, (int *)&isLinked);

	glUseProgram(testing);

	/* start the event handler */
	glutMainLoop();
	return 0;

	glDetachShader(testing, vertShader.getID());
	glDetachShader(testing, fragShader.getID());

}