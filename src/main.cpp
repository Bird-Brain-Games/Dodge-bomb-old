#pragma once

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
#include "shaderLoader.h"
#include "GameObject.h"
#include "shaders.h"
#include "math.h"
#include "controller.h"

glm::mat4 rotationMatrix;

controller p1(0);
controller p2(1);


float x_bomb = -2.0f;

// Defines and Core variables
#define FRAMES_PER_SECOND 60
const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND; // Miliseconds per frame

int windowWidth = 1920;
int windowHeight = 1080;

float mousepositionX;
float mousepositionY;

float lastMousepositionX;
float lastMousepositionY;

bool assignment = false;

int score = 0;


glm::vec3 position = glm::vec3(-119.0f, 72.5f, 1.0f);

glm::vec3 character2_pos = glm::vec3(0.0f, 2.5f, 40.6f);
glm::vec3 sphere_pos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 sphere_pos2 = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 bomb_acceleration = glm::vec3(0.0f, 1.0f, 1.0f);

// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 44.4000092;

float playerSpeed = 0.4f; // the players speed. used to how fast animation needs to be.

float speed = 0.3f; // 1 units / second
float mouseSpeed = 0.005f;


// A few conversions to know
float degToRad = 3.14159f / 180.0f;
float radToDeg = 180.0f / 3.14159f;

int lerpStage = 0;

bool bomb = false;


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

glm::vec3 score_position = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 rotation = glm::vec3(0.0f);
float dt;
float sphereTimer = 0;
UINT testing;

CShader vertShader;
CShader fragShader;
vector<GameObject> object;
vector<GameObject> UI;
vector<glm::vec3> dimensions;
// chest, robot, floor, robot2
vector<boundingBox> boundingBoxes;
PlayerObject animation[2];

Collision aabb;

CShader shVertex, shFragment;
CShaderProgram spMain;

float time = 0.0f;

glm::vec3 up;
glm::vec3 direction;

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

vector<GLuint> texture_handler_score;
vector<GLuint> texture_sampler_score;

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
		horizontalAngle = 1.57000148;
		verticalAngle = -0.520000100;
	}
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);
	up = glm::cross(right, direction);

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
float scale = 1;

/*
Binds the object data to the stuff, i can't really remember exactly what it does
but it allocates in memory for the object and stuff, so that's good.
- VAO
- index of VAO
- VBO
- object being bound
*/


void initScene()
{
	UI.push_back(GameObject("obj\\score.obj", "img\\score.png"));
	UI.push_back(GameObject("obj\\number.obj"));

	dimensions.push_back(glm::vec3(1.1f, 1.1f, 0.90f));// Chest
	dimensions.push_back(glm::vec3(2.1f*scale, 5.0f*scale, 2.2f*scale));// Robot
	dimensions.push_back(glm::vec3(41.5f, 0.05f, 41.5f));// floor
	dimensions.push_back(glm::vec3(0.44f, 0.47f, 0.44f));//sphere

	boundingBox temp;
	temp.max = glm::vec3(0) + dimensions[0];
	temp.min = glm::vec3(0) - dimensions[0];
	boundingBoxes.push_back(temp);
	temp.max = animation[0].getPos() + dimensions[1];//bot 1
	temp.min = animation[0].getPos() - dimensions[1];
	boundingBoxes.push_back(temp);
	temp.max = glm::vec3(0.0f, -3.0f, 0.0f) + dimensions[2];//table 2 
	temp.min = glm::vec3(0.0f, -3.0f, 0.0f) - dimensions[2];
	boundingBoxes.push_back(temp);
	temp.max = animation[0].getPos() + dimensions[1];//bot2 3
	temp.min = animation[0].getPos() - dimensions[1];
	boundingBoxes.push_back(temp);
	temp.max = animation[0].bomb.getPos() + dimensions[3];//bomb 4 
	temp.min = animation[0].bomb.getPos() - dimensions[3];
	boundingBoxes.push_back(temp);
	temp.max = animation[0].bomb.getPos() + dimensions[3];//bomb2  5
	temp.min = animation[0].bomb.getPos() - dimensions[3];
	boundingBoxes.push_back(temp);

	animation[0] = PlayerObject("obj\\robot\\base.obj", "img\\Bombot.jpg", "obj\\bomb.obj", "img\\bPileDiffuse.png", 4);
	animation[0].addAnim("obj\\robot\\robot_walk_anim.txt");
	animation[0].setCurrentAnim(1);

	animation[1] = PlayerObject("obj\\robot\\base.obj", "img\\Bombot2.jpg", "obj\\bomb.obj", "img\\bPileDiffuse.png", 5);
	animation[1].addAnim("obj\\robot\\robot_walk_anim.txt");
	animation[1].setCurrentAnim(1);

	object.push_back(GameObject("obj\\robot\\base.obj", "img\\Bombot.jpg"));
	sphereSpot = object.size();
	object.push_back(GameObject("obj\\bomb.obj", "img\\bPileDiffuse.png"));
	object.push_back(GameObject("obj\\table.obj", "img\\table_temp.jpg"));
	object.push_back(GameObject("obj\\robot\\base2.obj", "img\\Bombot2.jpg"));
	object.push_back(GameObject("obj\\bomb.obj", "img\\bPileDiffuse.png"));

	object[0].bindObjectData();
	object[1].bindObjectData();
	object[2].bindObjectData();
	object[3].bindObjectData();
	object[4].bindObjectData();

	animation[0].bindObjectData();
	animation[1].bindObjectData();

	UI[0].bindObjectData();
	UI[1].bindObjectData();

	char sTemp[] = "img\\num\\0.png";
	for (int count = 0; count < 10; count++)
	{
		sTemp[8] = count + 47;
		texture_handler_score.push_back(0);
		texture_sampler_score.push_back(0);
		texture_handler_score[count] = ilutGLLoadImage(sTemp);

		glBindTexture(GL_TEXTURE_2D, texture_handler_score[count]);


		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP),
			ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
			0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData()); /* Texture specification */
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_SMOOTH);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_SMOOTH);

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


	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

bool animate = false;
bool dirForward = true;

void test();
void drawUI();
void DisplayCallbackFunction(void)
{
	//glViewport(0, 0, windowWidth, windowHeight / 2);

	glClearColor(0.0f, 0.2f, 0.3f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, windowWidth, windowHeight);

	gluPerspective(initialFoV, windowWidth / windowHeight, 0.1f, 10000.0f);
	test();


	glViewport(0, 0, windowWidth, windowHeight);


	glOrtho(0, windowWidth, 0, windowHeight, -1.0, 1.0);//you can use negative nears and fars because of the way its math works.
	drawUI();



	//glViewport(0, windowHeight / 2, windowWidth, windowHeight / 2);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(initialFoV, windowWidth / windowHeight, 0.1f, 10000.0f);
	////test();

	//glViewport(0, windowHeight/2, windowWidth, windowHeight / 2);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0, windowWidth, 0, windowHeight, -1.0, 1.0);//you can use negative nears and fars because of the way its math works.
	//drawUI();
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glutSwapBuffers();

}
glm::vec3 UI_pos(0.0f, 0.0f, 0.0f);
void drawUI()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	makeMatricies();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 identity = glm::mat4(1.0);

	glm::mat4 mvp = glm::mat4(
		0.2f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.2f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.2f, 0.0f,
		-0.6f, 0.6f, 0.0f, 1.0f);

	int iModelViewProjectionLoc = glGetUniformLocation(spMain.getProgramID(), "modelViewProjectionMatrix");
	int sampler = glGetUniformLocation(spMain.getProgramID(), "gSampler");
	glUniform1i(sampler, 0);
	glActiveTexture(GL_TEXTURE0);


	UI[0].draw(iModelViewProjectionLoc, mvp);


	mvp = glm::mat4(
		0.2f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.2f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.2f, 0.0f,
		-0.1f, 0.6f, 0.0f, 1.0f);

	if (score >= 10)
	{
		glBindTexture(GL_TEXTURE_2D, texture_handler_score[(score + 1) % 10]);
		glBindVertexArray(UI[1].getVAO()[1]);
		glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
		glDrawArrays(GL_TRIANGLES, 0, UI[1].getBaseLoader().getVertices().size());
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture_handler_score[score + 1]);
		glBindVertexArray(UI[1].getVAO()[1]);
		glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
		glDrawArrays(GL_TRIANGLES, 0, UI[1].getBaseLoader().getVertices().size());
	}
}

void test()
{



	if (aabb.collisionAABB(boundingBoxes[1], boundingBoxes[2]) == true)
	{
		//std::cout << "collison!!" << std::endl;
	}
	else
		animation[0].addPos(glm::vec3(0.0f, -0.1f, 0.0f));


	glClearColor(0.0f, 0.2f, 0.3f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	int iModelViewProjectionLoc = glGetUniformLocation(spMain.getProgramID(), "modelViewProjectionMatrix");
	int sampler = glGetUniformLocation(spMain.getProgramID(), "gSampler");

	//glUniform1i(sampler, 0);
	//glActiveTexture(GL_TEXTURE0);

	makeMatricies();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 identity = glm::mat4(1.0);
	glm::mat4 mvp = ProjectionMatrix * ViewMatrix * identity;

	for (int i = 0; i < object.size(); i++)
	{
		//object[i].draw(iModelViewProjectionLoc, mvp);
	}


	mvp = ProjectionMatrix * ViewMatrix * identity;

	mvp = ProjectionMatrix * ViewMatrix * glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, -3.0f, 0.0f, 1.0f);


	object[2].draw(iModelViewProjectionLoc, mvp);


	glm::mat4  ModelMatrix = glm::mat4{
		scale, 0.0f, 0.0f, 0.0f,
		0.0f, scale, 0.0f, 0.0f,
		0.0f, 0.0f, scale, 0.0f,
		animation[0].getPos().x,  animation[0].getPos().y,  animation[0].getPos().z,  1.0f };

	rotationMatrix = glm::mat4{
		cos(rotation.y), 0.0f, sin(rotation.y), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(rotation.y), 0.0f, cos(rotation.y), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	ModelMatrix = ModelMatrix * rotationMatrix;

	mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
	if (animation[0].lives > 0)
		animation[0].draw(iModelViewProjectionLoc, mvp);


	ModelMatrix = glm::mat4{
	  scale, 0.0f, 0.0f, 0.0f,
	  0.0f, scale, 0.0f, 0.0f,
	  0.0f, 0.0f, scale, 0.0f,
	  animation[1].getPos().x,  animation[1].getPos().y,  animation[1].getPos().z,  1.0f };

	rotationMatrix = glm::mat4{
		cos(rotation.y), 0.0f, sin(rotation.y), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(rotation.y), 0.0f, cos(rotation.y), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	ModelMatrix = ModelMatrix * rotationMatrix;

	mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
	if (animation[1].lives > 0)
		animation[1].draw(iModelViewProjectionLoc, mvp);
	////glm::mat4 ModelMatrix = glm::mat4{
	////	1.0, 0.0f, 0.0f, 0.0f,
	////	0.0f, cos(90), -sin(90), 0.0f,
	////	0.0f, sin(90), cos(90), 0.0f,
	////	character_pos.x,  character_pos.y,  character_pos.z,  1.0f };

	//glm::mat4  ModelMatrix = glm::mat4{
	//scale, 0.0f, 0.0f, 0.0f,
	//0.0f, scale, 0.0f, 0.0f,
	//0.0f, 0.0f, scale, 0.0f,
	//character_pos.x,  character_pos.y,  character_pos.z,  1.0f };

	//glm::mat4 mvp = ProjectionMatrix * ViewMatrix * glm::mat4(
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	0.0f, -3.0f, 0.0f, 1.0f);



	//glBindTexture(GL_TEXTURE_2D, texture_handle[2]);
	//glBindVertexArray(uiVAO2[1]);
	//glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	//glDrawArrays(GL_TRIANGLES, 0, object[sphereSpot + 1].getVertex().size());
	for (int i = 0; i < 2; i++)
	{
		if (animation[i].bombThrow == true && animation[i].bombTimer > 0)
		{

			animation[i].bombTimer += dt;

			glm::mat4 mvp = ProjectionMatrix * ViewMatrix * glm::mat4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				animation[i].bomb.getPos().x, animation[i].bomb.getPos().y, animation[i].bomb.getPos().z, 1.0f);

			//sphere.x++;
			//sphere.y++;

			animation[i].bomb.addPos(animation[i].bomb.getVel());
			animation[i].bomb.addVel(glm::vec3(0.0f, -0.05f, 0.0f));

			if (animation[i].lives > 0)
				animation[i].bomb.draw(iModelViewProjectionLoc, mvp);

			if (animation[i].bombTimer > 1.5)
			{
				animation[i].bombTimer = 0;
			}
			int spot = 0;
			int spot2 = 0;
			if (i == 0)
			{
				spot = 4;
				spot2 = 3;
			}
			else
			{
				spot = 5;
				spot2 = 1;
			}

			boundingBoxes[spot].max = animation[i].bomb.getPos() + dimensions[3];
			boundingBoxes[spot].min = animation[i].bomb.getPos() - dimensions[3];
			if (aabb.collisionAABB(boundingBoxes[spot], boundingBoxes[spot2]))//checks whether we hit the other person
			{
				//std::cout << "score" << std::endl;
				animation[i].bombThrow = false;
				animation[i].bombTimer = 0;
				animation[i].score++;
				if (i = 1)
					animation[1].lives--;
				else
					animation[0].lives--;
			}
			if (aabb.collisionAABB(boundingBoxes[spot], boundingBoxes[2])) // checks whether we hit the floor
			{
				animation[i].bombThrow = false;
				animation[i].bombTimer = 0;
			}
		}
		//if (aabb.collisionAABB(boundingBoxes[0], boundingBoxes[2]))//??
		//{
		//	x_bomb -= 0.3f;
		//}

	}


	//glBindVertexArray(uiVAO[0]);
	//mvp = ProjectionMatrix * ViewMatrix * glm::mat4(
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	0.0f, x_bomb, 0.0f, 1.0f);
	////glBindSampler(0, texture_sampler[0]);
	//glBindTexture(GL_TEXTURE_2D, texture_handle[0]);


	//glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	//glDrawArrays(GL_TRIANGLES, 0, object[0].getVertex().size());

	//glBindVertexArray(uiVAO[1]);

	//glBindTexture(GL_TEXTURE_2D, texture_handle[1]);

	////character player 1
	//mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

	//glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	//glDrawArrays(GL_TRIANGLES, 0, object[1].getVertex().size());

	//glBindVertexArray(uiVAO[2]);
	////character player 2
	//glBindTexture(GL_TEXTURE_2D, texture_handle[3]);
	//mvp = ProjectionMatrix * ViewMatrix * glm::mat4(
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	character2_pos.x, character2_pos.y, character2_pos.z, 1.0f);
	//glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	//glDrawArrays(GL_TRIANGLES, 0, object[1].getVertex().size());

	boundingBoxes[1].max = animation[0].getPos() + dimensions[1];
	boundingBoxes[1].min = animation[0].getPos() - dimensions[1];

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

void characterInput(PlayerObject *player, controller conPlayer)
{
	glm::vec3 tempVel;
	tempVel = player->getVel();;
	Coords stick = conPlayer.getLeftStick();
	//std::cout << stick.x << "   " << stick.y << std::endl;
	if (keydown['j'] || stick.y > -0.1)
	{
		player->addPos(glm::vec3(playerSpeed, 0.0f, 0.0f));
		//	std::cout << "left" << std::endl;
	}
	if (keydown['l'] || stick.y < 0.1)
	{
		player->addPos(glm::vec3(-playerSpeed, 0.0f, 0.0f));
		//	std::cout << "right" << std::endl;
	}
	if (keydown['u'])
	{
		player->addPos(glm::vec3(0.0f, playerSpeed, 0.0f));
	}
	if (keydown['o'])
	{
		player->addPos(glm::vec3(0.0f, -playerSpeed, 0.0f));
	}
	if (keydown['i'] || stick.x > 0.1)
	{
		player->addPos(glm::vec3(0.0f, 0.0f, playerSpeed));
		//	std::cout << "up" << std::endl;
	}
	if (keydown['k'] || stick.x < -0.1)
	{
		player->addPos(glm::vec3(0.0f, 0.0f, -playerSpeed));
		//	std::cout << "down" << std::endl;
	}


	if (keydown[32] || conPlayer.conButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		player->charge += 0.03;

		//glm::vec3 direction = glm::vec3(conPlayer.getRightStick().y, 1.0f, conPlayer.getRightStick().x);

		//direction = glm::normalize(direction);
		//player->bomb.setVel(glm::vec3(direction* player->charge));

		//while (player->bombTimer < 1.5)
		//{
		//	player->bomb.addPos(player->bomb.getVel());
		//	player->bombTimer += dt;
		//	std::cout << player->bom. << std::endl;
		//	if (aabb.collisionAABB(boundingBoxes[player->temp], boundingBoxes[2])) // checks whether we hit the floor
		//	{
		//		int iModelViewProjectionLoc = glGetUniformLocation(spMain.getProgramID(), "modelViewProjectionMatrix");
		//		int sampler = glGetUniformLocation(spMain.getProgramID(), "gSampler");

		//		//glUniform1i(sampler, 0);
		//		//glActiveTexture(GL_TEXTURE0);

		//		makeMatricies();
		//		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		//		glm::mat4 ViewMatrix = getViewMatrix();
		//		glm::mat4 mvp = ProjectionMatrix * ViewMatrix * glm::mat4(
		//			1.0f, 0.0f, 0.0f, 0.0f,
		//			0.0f, 1.0f, 0.0f, 0.0f,
		//			0.0f, 0.0f, 1.0f, 0.0f,
		//			player->bomb.getPos().x, player->bomb.getPos().y, player->bomb.getPos().z, 1.0f);

		//		player->bomb.draw(iModelViewProjectionLoc, mvp);
		//	}
		//	boundingBoxes[player->temp].max = player->bomb.getPos() + dimensions[3];
		//	boundingBoxes[player->temp].min = player->bomb.getPos() - dimensions[3];
		//}

		//player->bomb.setPos(player->getPos());
		//player->bomb.setVel(glm::vec3(0));
		//player->bombTimer = 0;

		//boundingBoxes[player->temp].max = player->bomb.getPos() + dimensions[3];
		//boundingBoxes[player->temp].min = player->bomb.getPos() - dimensions[3];
	}
	if (player->charge > 0 && conPlayer.conButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) == false)
	{
		glm::vec3 direction = glm::vec3(conPlayer.getRightStick().y, 1.0f, conPlayer.getRightStick().x);
		direction = glm::normalize(direction);
		player->bomb.setVel(glm::vec3(direction* player->charge));
		player->bombThrow = true;
		player->bomb.setPos(player->getPos());
		player->bombTimer += dt;
		player->charge = 0;
	}
	//if (player.z > tempZ)
	//{
	//	animate = true;
	//	dirForward = true;
	//}
	//else if (character_pos.z < tempZ)
	//{
	//	animate = true;
	//	dirForward = true;
	//}
	//else
	//	animate = false;

}
bool selected = true;
void processInputs()
{
	//std::cout << p1.connected() << std::endl;

	if (selected == true)
	{
		characterInput(&animation[0], p1);
		characterInput(&animation[1], p2);
	}
	else
	{
		characterInput(&animation[0], p2);
		characterInput(&animation[1], p1);
	}
	//fg ht ry

	if (keydown['3'])
		selected = false;
	if (keydown['4'])
		selected = true;

	if (keydown['f'])
	{
		UI_pos.x -= playerSpeed;
	}
	if (keydown['h'])
	{
		UI_pos.x += playerSpeed;
	}
	if (keydown['t'])
	{
		UI_pos.y += playerSpeed;
	}
	if (keydown['g'])
	{
		UI_pos.y -= playerSpeed;
	}
	if (keydown['y'])
	{
		UI_pos.z += playerSpeed;
	}
	if (keydown['r'])
	{
		UI_pos.z -= playerSpeed;
	}



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

	if (keydown['1'])
	{
		initialFoV += 0.05f;
	}
	if (keydown['2'])
	{
		initialFoV -= 0.05f;
	}

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

	// Process all inputs
	processInputs();

	static int elapsedTimeAtLastTick = 0;
	int totalElapsedTime = glutGet(GLUT_ELAPSED_TIME);

	dt = totalElapsedTime - elapsedTimeAtLastTick;
	dt /= 1000.0f;
	elapsedTimeAtLastTick = totalElapsedTime;

	// Update game objects
	for (GameObject & o : object)
	{
		o.update(dt);
	}
	//animation[0].update(dt);

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
	//glViewport(0, 0, windowWidth, windowHeight);
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

