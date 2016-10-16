﻿// Core Libraries
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
#include "math.h"


// Defines and Core variables
#define FRAMES_PER_SECOND 60
const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND; // Miliseconds per frame

int windowWidth = 800;
int windowHeight = 600;

float mousepositionX;
float mousepositionY;

float lastMousepositionX;
float lastMousepositionY;


glm::vec3 position = glm::vec3(-9.6f, 28.7f, -75.0f);
glm::vec3 character_pos = glm::vec3(0.0f, -0.35f, -40.6f);
glm::vec3 character2_pos = glm::vec3(0.0f, -0.35f, 40.6f);
glm::vec3 sphere_pos = glm::vec3(0.0f, 0.0f, 0.0f);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float playerSpeed = 0.2f; // the players speed. used to how fast animation needs to be.

float speed = 0.3f; // 1 units / second
float mouseSpeed = 0.005f;


// A few conversions to know
float degToRad = 3.14159f / 180.0f;
float radToDeg = 180.0f / 3.14159f;

bool lerpStage = true;

bool bomb = false;


/* function DisplayCallbackFunction(void)
* Description:
*  - this is the openGL display routine
*  - this draws the sprites appropriately
*/


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

int sphereSpot;

bool lock = false; //locks the mouse to the center of the screen;
float lastTime;
UINT uiVBO[4];
UINT uiVAO[2];

UINT uiVBO2[4];
UINT uiVAO2[2];

float rotation = 0;
float dt;
float sphereTimer = 0;
UINT testing;

CShader vertShader;
CShader fragShader;
vector<Loader> object;
vector<glm::vec3> dimensions;
// chest, robot, floor, robot2
vector<boundingBox> boundingBoxes;
vector<Loader> animation;

Collision aabb;

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
	ProjectionMatrix = glm::perspective(initialFoV, (float)windowWidth / windowHeight, 0.1f, 10000.0f);
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
	object[0].load("obj\\chest_closed.obj");

	dimensions.push_back(glm::vec3(1.2f, 1.05f, 1.15f));// Chest
	dimensions.push_back(glm::vec3(2.35f, 2.7f, 1.45f));// Robot
	dimensions.push_back(glm::vec3(41.5f, 0.05f, 41.5f));// floor
	dimensions.push_back(glm::vec3(0.44f, 0.47f, 0.44f));//sphere

	boundingBox temp;
	temp.max = glm::vec3(0) + dimensions[0];
	temp.min = glm::vec3(0) - dimensions[0];
	boundingBoxes.push_back(temp);
	temp.max = character_pos + dimensions[1];
	temp.min = character_pos - dimensions[1];
	boundingBoxes.push_back(temp);
	temp.max = glm::vec3(0.0f, -3.0f, 0.0f) + dimensions[2];
	temp.min = glm::vec3(0.0f, -3.0f, 0.0f) - dimensions[2];
	boundingBoxes.push_back(temp);
	temp.max = character2_pos + dimensions[1];
	temp.min = character2_pos - dimensions[1];
	boundingBoxes.push_back(temp);
	temp.max = sphere_pos + dimensions[3];
	temp.min = sphere_pos - dimensions[3];
	boundingBoxes.push_back(temp);


	object[1].load("obj\\simple_man_base.obj");
	animation.push_back(Loader());
	animation.push_back(Loader());
	animation[0].load("obj\\simple_man_base.obj");
	animation[1].load("obj\\simple_man_walk.obj");

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

	sphereSpot = object.size();
	object.push_back(Loader());
	object[sphereSpot].load("obj\\bomb.obj");
	object.push_back(Loader());
	object[sphereSpot + 1].load("obj\\plane.obj");
	glGenVertexArrays(2, uiVAO2);
	glGenBuffers(4, uiVBO2);


	glBindVertexArray(uiVAO2[0]);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO2[0]);
	glBufferData(GL_ARRAY_BUFFER, object[sphereSpot].getVertex().size() * sizeof(glm::vec3), object[sphereSpot].getVertex().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO2[1]);
	glBufferData(GL_ARRAY_BUFFER, object[sphereSpot].getUV().size() * sizeof(glm::vec2), object[sphereSpot].getUV().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(uiVAO2[1]);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO2[2]);
	glBufferData(GL_ARRAY_BUFFER, object[sphereSpot + 1].getVertex().size() * sizeof(glm::vec3), object[sphereSpot + 1].getVertex().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO2[3]);
	glBufferData(GL_ARRAY_BUFFER, object[sphereSpot + 1].getUV().size() * sizeof(glm::vec2), object[sphereSpot + 1].getUV().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


	glBindVertexArray(uiVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, object[1].getVertex().size() * sizeof(glm::vec3), object[1].getVertex().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, object[1].getUV().size() * sizeof(glm::vec2), object[1].getUV().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


	texture_handle.push_back(0);
	texture_handle.push_back(0);
	texture_handle.push_back(0);

	texture_sampler.push_back(0);
	texture_sampler.push_back(0);
	texture_sampler.push_back(0);

	glGenTextures(3, &texture_handle[0]);
	glGenSamplers(3, &texture_sampler[0]);

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

	texture_handle[2] = ilutGLLoadImage("img\\floor.jpg");
	glBindTexture(GL_TEXTURE_2D, texture_handle[2]);



	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP),
		ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData()); /* Texture specification */

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


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
			time += dt * (1 + playerSpeed);
		else
			time -= dt * (1 + playerSpeed);
		//std::cout << time << std::endl;

		glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
		for (int count = 0; count < object[1].getVertex().size(); count++)
		{
			if (lerpStage == true)
				object[1].getVertex()[count] = lerp<glm::vec3>(animation[0].getVertex()[count], animation[1].getVertex()[count], time);
			else
				object[1].getVertex()[count] = lerp<glm::vec3>(animation[1].getVertex()[count], animation[0].getVertex()[count], time);
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, object[1].getVertex().size() * sizeof(glm::vec3), object[1].getVertex().data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	//gravity
	if (aabb.collisionAABB(boundingBoxes[1], boundingBoxes[2]) == true)
	{
		
	}
	else
	{
		character_pos.y -= 0.01;
	}
	//if (boundingBoxes[3])
	//std::cout << object.getVertex()[0].x << std::endl;
	//std::cout << lerp<glm::vec3>(object2.getVertex()[0], object3.getVertex()[0], time).x << std::endl;
	glLoadIdentity();
	rotation += 1;

	/* clear the screen */

	glClearColor(0.0f, 0.2f, 0.3f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	int iModelViewProjectionLoc = glGetUniformLocation(spMain.getProgramID(), "modelViewProjectionMatrix");
	int sampler = glGetUniformLocation(spMain.getProgramID(), "gSampler");
	glUniform1i(sampler, 0);
	glActiveTexture(GL_TEXTURE0);

	makeMatricies();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 identity = glm::mat4(1.0);


	glm::mat4 ModelMatrix = glm::mat4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		character_pos.x,  character_pos.y,  character_pos.z,  1.0f };

	glm::mat4 mvp = ProjectionMatrix * ViewMatrix * glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, -3.0f, 0.0f, 1.0f);;



	glBindTexture(GL_TEXTURE_2D, texture_handle[2]);
	glBindVertexArray(uiVAO2[1]);
	glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, 0, object[sphereSpot + 1].getVertex().size());

	if (bomb == true && sphereTimer > 0)
	{
		glBindTexture(GL_TEXTURE_2D, texture_handle[0]);
		glBindVertexArray(uiVAO2[0]);
		sphereTimer += dt;

		glm::mat4 mvp = ProjectionMatrix * ViewMatrix * glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			sphere_pos.x, sphere_pos.y, sphere_pos.z, 1.0f);
		//sphere.x++;
		//sphere.y++;
		sphere_pos.z++;
		glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
		glDrawArrays(GL_TRIANGLES, 0, object[sphereSpot].getVertex().size());
		if (sphereTimer > 1.5)
		{
			sphereTimer = 0;
		}
		boundingBoxes[4].max = sphere_pos + dimensions[3];
		boundingBoxes[4].min = sphere_pos - dimensions[3];
		if (aabb.collisionAABB(boundingBoxes[4], boundingBoxes[3]))//checks whether we hit the other person
		{
			std::cout << "score" << std::endl;
		}
	}


	glBindVertexArray(uiVAO[0]);
	
	//glBindSampler(0, texture_sampler[0]);
	glBindTexture(GL_TEXTURE_2D, texture_handle[0]);


	glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, 0, object[0].getVertex().size());

	glBindVertexArray(uiVAO[1]);

	glBindTexture(GL_TEXTURE_2D, texture_handle[1]);

	//character player 1
	mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

	glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, 0, object[1].getVertex().size());

	//character player 2
	mvp = ProjectionMatrix * ViewMatrix * glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		character2_pos.x, character2_pos.y, character2_pos.z, 1.0f );
	glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, 0, object[1].getVertex().size());

	boundingBoxes[1].max = character_pos + dimensions[1];
	boundingBoxes[1].min = character_pos - dimensions[1];



	/* Swap Buffers to Make it show up on screen */
	glutSwapBuffers();

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
	tempX = character_pos.x;
	tempY = character_pos.y;
	tempZ = character_pos.z;
	if (keydown['j'])
	{
		character_pos.x += playerSpeed;
	}
	if (keydown['l'])
	{
		character_pos.x -= playerSpeed;
	}
	if (keydown['u'])
	{
		character_pos.y += playerSpeed;
	}
	if (keydown['o'])
	{
		character_pos.y -= playerSpeed;
	}
	if (keydown['i'])
	{
		character_pos.z += playerSpeed;
	}
	if (keydown['k'])
	{
		character_pos.z -= playerSpeed;
	}


	if (character_pos.z > tempZ)
	{
		animate = true;
		dirForward = true;
	}
	else if (character_pos.z < tempZ)
	{
		animate = true;
		dirForward = false;
	}
	else
		animate = false;

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
	if (keydown[32])
	{
		if (sphereTimer <= 0)
		{
			bomb = true;
			sphere_pos.x = character_pos.x;
			sphere_pos.y = character_pos.y;
			sphere_pos.z = character_pos.z;
			sphereTimer += dt;
		}

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
	gluPerspective(45.0f, (float)w / h, 0.1f, 10000.0f);
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