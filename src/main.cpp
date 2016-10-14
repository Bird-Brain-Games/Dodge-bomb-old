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


#include "IL\ilut.h"
//Project files
#include "objLoader.h"
#include "shaderLoader.h"
#include "shaders.h"
#include "lerp.h"

// Defines and Core variables
#define FRAMES_PER_SECOND 60
const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND; // Miliseconds per frame

int windowWidth = 800;
int windowHeight = 600;

float mousepositionX;
float mousepositionY;

float lastMousepositionX;
float lastMousepositionY;

// Initial position : on +Z
glm::vec3 position = glm::vec3(-2.7f, 17.5f, -25.0f);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 0.3f; // 1 units / second
float mouseSpeed = 0.005f;


// A few conversions to know
float degToRad = 3.14159f / 180.0f;
float radToDeg = 180.0f / 3.14159f;

bool lerpStage = true;


/* function DisplayCallbackFunction(void)
* Description:
*  - this is the openGL display routine
*  - this draws the sprites appropriately
*/

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

float g = 0;
float r = 0.2;
float b = 0.2;

bool lock = false; //locks the mouse to the center of the screen;
float lastTime;
UINT uiVBO[4]; // One VBO for vertices positions, one for colors
UINT uiVAO[2]; // One VAO for pyramid

float rotation = 0;
float dt;
UINT testing;

CShader vertShader;
CShader fragShader;
vector<Loader> object;

CShader shVertex, shFragment;
CShaderProgram spMain;

float time = 0.0f;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

vector<GLuint> texture_handle;
vector<GLuint> texture_sampler;
bool cameraLock = true;
void makeMatricies()
{
	if (cameraLock == false)
	{
		horizontalAngle += mouseSpeed * float(lastMousepositionX - mousepositionX);
		verticalAngle += mouseSpeed * float(lastMousepositionY - mousepositionY);
	}
	else
	{
		horizontalAngle = 0.0599999987;
		verticalAngle = -0.219999850;
	}
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);
	glm::vec3 up = glm::cross(right, direction);

	if (keydown['w'])
	{
		position += direction *  speed;
	}
	// Move backward
	if (keydown['s'])
	{
		position -= direction *  speed;
	}
	// Strafe right
	if (keydown['d'])
	{
		position += right *  speed;
	}
	// Strafe left
	if (keydown['a'])
	{
		position -= right *  speed;
	}


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(initialFoV, (float)windowWidth / windowHeight, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

}
void* ptr;
void initScene()
{
	object.push_back(Loader());
	object.push_back(Loader());
	object.push_back(Loader());
	object.push_back(Loader());
	object[0].load("obj\\chest_closed.obj");
	object[1].load("obj\\simple_man_base.obj");
	object[2].load("obj\\simple_man_walk.obj");
	object[3].load("obj\\simple_man_base.obj");
	glGenVertexArrays(2, uiVAO);
	glGenBuffers(4, uiVBO);

	glBindVertexArray(uiVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, object[0].getVertex().size() * sizeof(glm::vec3), object[0].getVertex().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, object[0].getUV().size() * sizeof(glm::vec2), object[0].getUV().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);



	glBindVertexArray(uiVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, object[3].getVertex().size() * sizeof(glm::vec3), object[3].getVertex().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, object[3].getUV().size() * sizeof(glm::vec2), object[3].getUV().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


	texture_handle.push_back(0);
	texture_handle.push_back(0);
	texture_sampler.push_back(0);
	texture_sampler.push_back(0);

	glGenTextures(2, &texture_handle[0]);
	glGenSamplers(2, &texture_sampler[0]);

	glBindTexture(GL_TEXTURE_2D, texture_handle[0]);

	texture_handle[0] = ilutGLLoadImage("img\\diffuse.png");

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP),
		ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData()); /* Texture specification */


	texture_handle[1] = ilutGLLoadImage("diffuse.png");
	glBindTexture(GL_TEXTURE_2D, texture_handle[1]);



	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP),
		ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData()); /* Texture specification */

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//object2.load("thor.obj");
	//
	//
	//glBindVertexArray(uiVAO[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	//glBufferData(GL_ARRAY_BUFFER, object2.getVertex().size() * sizeof(float), object2.getVertex().data(), GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
	//glBufferData(GL_ARRAY_BUFFER, object2.getColor().size() * sizeof(float), object2.getColor().data(), GL_STATIC_DRAW);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

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

bool animate = false;
bool dirForward = true;
void DisplayCallbackFunction(void)
{
	if (animate == true)
	{
		if (time > 1)
		{
			time = 0;
			lerpStage = !lerpStage;
		}
		else if (time < 0)
		{
			time = 1;
			lerpStage = !lerpStage;
		}
		if (dirForward == true)
			time += dt;
		else
			time -= dt;
		//std::cout << time << std::endl;

		glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
		for (int count = 0; count < object[3].getVertex().size(); count++)
		{
			if (lerpStage == true)
				object[3].getVertex()[count] = lerp<glm::vec3>(object[1].getVertex()[count], object[2].getVertex()[count], time);
			else
				object[3].getVertex()[count] = lerp<glm::vec3>(object[2].getVertex()[count], object[1].getVertex()[count], time);
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, object[3].getVertex().size() * sizeof(glm::vec3), object[3].getVertex().data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//std::cout << object.getVertex()[0].x << std::endl;
	//std::cout << lerp<glm::vec3>(object2.getVertex()[0], object3.getVertex()[0], time).x << std::endl;
	glLoadIdentity();
	rotation += 1;

	/* clear the screen */

	glClearColor(0.0f, 0.2f, 0.3f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	int iModelViewProjectionLoc = glGetUniformLocation(spMain.getProgramID(), "modelViewProjectionMatrix");
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);


	makeMatricies();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 identity = glm::mat4(1.0);


	glm::mat4 ModelMatrix = glm::mat4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		tx,  ty,  tz,  1.0f };

	glm::mat4 mvp = ProjectionMatrix * ViewMatrix * identity;

	glBindVertexArray(uiVAO[0]);

	//glm::mat4 View = glm::lookAt(glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
	//	glm::vec3(0, 0, 0), // and looks at the origin
	//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	//);
	//glm::mat4 Model = glm::mat4(1.0f);
	//glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the 

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

	//texture_sampler[0] = glGetUniformLocation(spMain.getProgramID(), "gSampler");
	//glUniform1i(texture_sampler[0], 0);
	//glActiveTexture(GL_TEXTURE0 + 0);
	//glBindTexture(GL_TEXTURE_2D, texture_handle[0]);
	//glBindSampler(0, texture_sampler);



	int sampler = glGetUniformLocation(spMain.getProgramID(), "gSampler");
	glUniform1i(sampler, 0);
	glActiveTexture(GL_TEXTURE0);
	//glBindSampler(0, texture_sampler[0]);
	glBindTexture(GL_TEXTURE_2D, texture_handle[0]);

	glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, 0, object[0].getVertex().size());

	glBindVertexArray(uiVAO[1]);
	//texture_sampler[1] = glGetUniformLocation(spMain.getProgramID(), "gSampler");
	//glUniform1i(texture_sampler[1], 0);
	//glActiveTexture(GL_TEXTURE1);

	//glActiveTexture(GL_TEXTURE0 + 0);
//	glBindTexture(GL_TEXTURE_2D, texture_handle[1]);
	//glBindSampler(0, texture_sampler2);
	//glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_handle[1]);

	mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

	glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, 0, object[3].getVertex().size());

	//glBindVertexArray(uiVAO[1]);

	//glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	//glDrawArrays(GL_TRIANGLES, 0, object2.getVertex().size());

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
	//std::cout << "Key Down:" << (int)key << std::endl;

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
	float tempX, tempY, tempZ;
	tempX = tx;
	tempY = ty;
	tempZ = tz;
	if (keydown['l'])
	{
		tx += 0.05;
	}
	if (keydown['j'])
	{
		tx -= 0.05;
	}
	if (keydown['u'])
	{
		ty += 0.05;
	}
	if (keydown['o'])
	{
		ty -= 0.05;
	}
	if (keydown['i'])
	{
		tz += 0.05;
	}
	if (keydown['k'])
	{
		tz -= 0.05;
	}


	if (tz > tempZ)
	{
		animate = true;
		dirForward = true;
	}
	else if (tz < tempZ)
	{
		animate = true;
		dirForward = false;
	}
	else
		animate = false;

	std::cout << "animate: " << animate << " Zstart: " << tempZ << " Zend " << tz << std::endl;

	if (keydown['z'])
	{
		lock = !lock;
	}
	if (keydown['q'])
	{
		glutLeaveMainLoop();
	}
	if (lock == true)
	{
		glutWarpPointer(windowWidth / 2, windowHeight / 2);
	}
	if (keydown['x'])
	{
		animate = !animate;
	}
	if (keydown['c'])
	{
		cameraLock = !cameraLock;
	}

	static int elapsedTimeAtLastTick = 0;
	int totalElapsedTime = glutGet(GLUT_ELAPSED_TIME);

	dt = totalElapsedTime - elapsedTimeAtLastTick;
	dt /= 1000.0f;
	elapsedTimeAtLastTick = totalElapsedTime;

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
	lastMousepositionX = mousepositionX;
	lastMousepositionY = mousepositionY;
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

	glEnable(GL_TEXTURE_2D);
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);



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