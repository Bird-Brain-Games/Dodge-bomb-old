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
#include "MultiTexObject.h"
#include "shaders.h"
#include "math.h"
#include "InputManager.h"
#include "controller.h"

glm::mat4 rotationMatrix;

controller p1(0);
controller p2(1);

GameObject *line;
GameObject line2;
glm::vec3 crosshairPos[2];
float incriment = 0.1;

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

glm::vec3 direction;
glm::vec3 position = glm::vec3(-119.0f, 72.5f, 1.0f);
glm::vec3 gameDefaultPos = glm::vec3(-119.0f, 72.5f, 1.0f);
glm::vec3 menuDefaultPos = glm::vec3(-155.0f, 80.0f, 1.0f);
glm::vec2 gameDefaultAngle(1.57000148f, -0.520000100);
glm::vec2 menuDefaultAngle(1.57f, -0.140f);
glm::vec2 currentAngles = gameDefaultAngle;


bool inMenu;
bool isReady;
bool inMainMenu;

//glm::vec3 character2_pos = glm::vec3(0.0f, 2.5f, 40.6f);
//glm::vec3 sphere_pos = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 sphere_pos2 = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 bomb_acceleration = glm::vec3(0.0f, 1.0f, 1.0f);

// Initial Field of View
float initialFoV = 44.4000092;

float playerSpeed = 24.0f; // the players speed. used to how fast animation needs to be.

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


vector<GameObject> object;
vector<MultiObject> UI;
vector<GameObject> menuObjects;
vector<AnimatedObject> menuAnimations;

PlayerObject animation[2];

Collision aabb;

ShaderLoader shVertex, shFragment, weightVert, weightFrag;
ShaderProgram spMain, weightMain;


glm::vec3 up;


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
		currentAngles.x += mouseSpeed * float(lastMousepositionX - mousepositionX);
		currentAngles.y += mouseSpeed * float(lastMousepositionY - mousepositionY);
	}

	direction = glm::vec3(
		cos(currentAngles.y) * sin(currentAngles.x),
		sin(currentAngles.y),
		cos(currentAngles.y) * cos(currentAngles.x)
	);
	glm::vec3 right = glm::vec3(
		sin(currentAngles.x - 3.14f / 2.0f),
		0,
		cos(currentAngles.x - 3.14f / 2.0f)
	);
	up = glm::cross(right, direction);

	if (KEYBOARD_INPUT->IsKeyDown('w'))
	{
		position += direction *  speed;
	}
	// Move backward
	if (KEYBOARD_INPUT->IsKeyDown('s'))
	{
		position -= direction *  speed;
	}
	// Strafe right
	if (KEYBOARD_INPUT->IsKeyDown('d'))
	{
		position += right *  speed;
	}
	// Strafe left
	if (KEYBOARD_INPUT->IsKeyDown('a'))
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
float scale = 1.0;



void initScene()
{
	inMenu = false;


	//UI.push_back(MultiTexObject("obj\\paper.obj", "img\\menu\\paperDiff.jpg", glm::vec3(1.0f)));	// "Ready?" note
	//UI.push_back(MultiTexObject("obj\\paper.obj", "img\\menu\\blueNOREADY.jpg", glm::vec3(1.0f)));	// Blue note
	//UI.push_back(MultiTexObject("obj\\paper.obj", "img\\menu\\redNOREADY.jpg", glm::vec3(1.0f)));	// Red note

	//UI[1].addTex("img\\menu\\blueREADY.jpg");
	//UI[1].addTex("img\\menu\\tacnoteBLUEDif.jpg");
	//UI[1].addTex("img\\menu\\tacnoteBLUExDif.jpg");

	//UI[2].addTex("img\\menu\\blueREADY.jpg");
	//UI[2].addTex("img\\menu\\tacnoteBLUEDif.jpg");
	//UI[2].addTex("img\\menu\\tacnoteBLUExDif.jpg");
	//
	//UI[0].bindObjectData();

	//dimensions.push_back(glm::vec3(1.1f, 1.1f, 0.90f));// Chest
	//dimensions.push_back(glm::vec3(2.1f*scale, 5.0f*scale, 2.2f*scale));// Robot
	//dimensions.push_back(glm::vec3(41.5f, 0.05f, 41.5f));// floor
	//dimensions.push_back(glm::vec3(0.44f, 0.47f, 0.44f));//sphere

	animation[0] = PlayerObject("obj\\robot\\bombot2_test.obj", "img\\bombot(diffuse).png", 0, glm::vec3(2.1f, 5.0f, 2.2f));
	animation[0].addAnim("obj\\robot\\robot_walk_anim.txt");
	animation[0].setCurrentAnim(1);
	animation[0].setPos(glm::vec3(0.0, 10.0, 5.0));
	animation[0].setMass(1.5f);
	animation[0].useGravity(true);


	animation[1] = PlayerObject("obj\\robot\\bombot2_test.obj", "img\\bombot(diffuse)2.png", 1, glm::vec3(2.1f, 5.0f, 2.2f));
	animation[1].addAnim("obj\\robot\\robot_walk_anim.txt");
	animation[1].setCurrentAnim(1);
	animation[1].setPos(glm::vec3(0.0, 10.0, -5.0));
	animation[1].setMass(1.5f);
	animation[1].useGravity(true);

	object.push_back(GameObject("obj\\desk.obj", "img\\desk (diffuse).png", glm::vec3(41.5f, 1.0f, 41.5f)));
	object[0].bindObjectData(GL_STATIC_DRAW);

	object.push_back(GameObject("obj\\room.obj", "img\\wall and floor (diffuse).png", glm::vec3(41.5f, 0.05f, 41.5f)));
	object[1].bindObjectData(GL_STATIC_DRAW);
	object.push_back(GameObject("obj\\wall.obj", "img\\wall and floor (diffuse).png", glm::vec3(41.5f, 0.05f, 41.5f)));
	object[2].bindObjectData(GL_STATIC_DRAW);
	object[2].setPos(glm::vec3(28.0f, 60.0f, 0.0f));

	// Initialize menu objects
	menuObjects.push_back(GameObject("obj\\board.obj", "img\\menu\\boardDiffuse.jpg", glm::vec3(0.024, 0.7855, 0.995)));
	menuObjects[0].bindObjectData(GL_STATIC_DRAW);
	menuObjects[0].setScale(glm::vec3(40.0f));
	menuObjects[0].setPos(glm::vec3(0.0f, 25.0f, 0.0f));

	animation[0].bindObjectData();
	animation[1].bindObjectData();

	animation[0].bomb.bindObjectData();
	animation[1].bomb.bindObjectData();



	//UI[0].bindObjectData();
	//UI[1].bindObjectData();

	line = new GameObject("obj//crosshair.obj", "img//redtargetdif.jpg", glm::vec3(1.0f));
	line->bindObjectData();
	line2 = GameObject("obj//crosshair.obj", "img//bluetargetdiff.jpg", glm::vec3(1.0f));
	line2.bindObjectData();

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



	if (weightVert.loadShader("shaders\\skinning.vert", GL_VERTEX_SHADER) == false)
	{

		weightVert.OutputProgramLog();
	}
	weightFrag.loadShader("shaders\\shader.frag", GL_FRAGMENT_SHADER);

	weightMain.createProgram();
	weightMain.addShader(&weightVert);

	weightMain.addShader(&weightFrag);

	weightMain.linkProgram();
	weightMain.useProgram();

	shVertex.loadShader("shaders\\shader.vert", GL_VERTEX_SHADER);
	shFragment.loadShader("shaders\\shader.frag", GL_FRAGMENT_SHADER);

	spMain.createProgram();
	spMain.addShader(&shVertex);
	spMain.addShader(&shFragment);

	spMain.linkProgram();
	spMain.useProgram();


	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	animation[0].bomb.setMass(0.3f);
	animation[1].bomb.setMass(0.3f);


}

bool animate = false;
bool dirForward = true;

void test();
void drawUI();
void drawMenu();
void DisplayCallbackFunction(void)
{
	//glViewport(0, 0, windowWidth, windowHeight / 2);

	glClearColor(0.0f, 0.2f, 0.3f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, windowWidth, windowHeight);

	gluPerspective(initialFoV, windowWidth / windowHeight, 0.1f, 10000.0f);
	test();
	drawMenu();

	glViewport(0, 0, windowWidth, windowHeight);


	//glOrtho(0, windowWidth, 0, windowHeight, -1.0, 1.0);//you can use negative nears and fars because of the way its math works.
	//drawUI();

	glutSwapBuffers();

}

glm::vec3 UI_pos(0.0f, 0.0f, 0.0f);

void drawMenu()
{
	int iModelViewProjectionLoc = glGetUniformLocation(spMain.getProgramID(), "modelViewProjectionMatrix");
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 identity = glm::mat4(1.0);
	glm::mat4 mvp; //= ProjectionMatrix * ViewMatrix * identity;
	glm::mat4  ModelMatrix, transMatrix, scaleMatrix;

	transMatrix = glm::translate(identity, menuObjects[0].getPos());
	scaleMatrix = glm::scale(identity, menuObjects[0].getScale());
	ModelMatrix = transMatrix * scaleMatrix;

	/*ModelMatrix = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		menuObjects[0].getPos().x, menuObjects[0].getPos().y - 1.0f, menuObjects[0].getPos().z, 1.0f);*/

	mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;


	for (int i = 0; i < menuObjects.size(); i++)
	{
		menuObjects[i].draw(iModelViewProjectionLoc, mvp);
	}
}

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


	

	
}

void test()
{

	spMain.useProgram();

	glm::mat4  ModelMatrix;



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



	ModelMatrix = glm::mat4(
		2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.5f, 0.0f,
		object[0].getPos().x, object[0].getPos().y - 1.0f, object[0].getPos().z, 1.0f);

	rotationMatrix = glm::mat4{
		cos(90 * degToRad), 0.0f, sin(90 * degToRad), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(90 * degToRad), 0.0f, cos(90 * degToRad), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	ModelMatrix = ModelMatrix * rotationMatrix;

	mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

	object[0].draw(iModelViewProjectionLoc, mvp);

	mvp = ProjectionMatrix * ViewMatrix * identity;

	object[1].draw(iModelViewProjectionLoc, mvp);

	mvp = ProjectionMatrix * ViewMatrix * glm::translate(identity, object[2].getPos()) * rotationMatrix;

	object[2].draw(iModelViewProjectionLoc, mvp);


	ModelMatrix = glm::mat4{
		scale, 0.0f, 0.0f, 0.0f,
		0.0f, scale, 0.0f, 0.0f,
		0.0f, 0.0f, scale, 0.0f,
		animation[0].getPos().x,  animation[0].getPos().y,  animation[0].getPos().z,  1.0f };

	glm::mat4 directionT = glm::translate(ModelMatrix, glm::vec3(0.0f, -4.0f, 0.0f));

	rotationMatrix = glm::mat4{
		cos(animation[0].getRot().y), 0.0f, sin(animation[0].getRot().y), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(animation[0].getRot().y), 0.0f, cos(animation[0].getRot().y), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	//rotationMatrix = glm::rotate(directionT, animation[0].getRot());
	glm::mat4 directionModel = directionT * rotationMatrix;
	mvp = ProjectionMatrix * ViewMatrix * directionModel;

	if (animation[0].charge > 0)
	{
		glm::vec3 playerPos(animation[0].getPos());
		glm::vec3 dir(-sin(animation[0].getRot().y), 0.0f, cos(animation[0].getRot().y));
		crosshairPos[0] = glm::vec3(playerPos.x, playerPos.y - 4.0f, playerPos.z) + (dir * 43.0f);
		line->setPos(lerp(playerPos, crosshairPos[0], animation[0].charge));

		mvp = ProjectionMatrix * ViewMatrix * glm::translate(identity, line->getPos()); //* directionModel;
		line->draw(iModelViewProjectionLoc, mvp);
	}


	ModelMatrix = ModelMatrix * rotationMatrix;


	mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
	if (animation[0].lives > 0)
	{
		if (!animation[0].isInvincible())
			animation[0].draw(iModelViewProjectionLoc, mvp, 0, 2490);

		ModelMatrix = glm::mat4{
			scale, 0.0f, 0.0f, 0.0f,
			0.0f, scale, 0.0f, 0.0f,
			0.0f, 0.0f, scale, 0.0f,
			animation[0].getPos().x,  animation[0].getPos().y,  animation[0].getPos().z,  1.0f };
		mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

		animation[0].draw(iModelViewProjectionLoc, mvp, 2490, 882);
	}


	ModelMatrix = glm::mat4{
		scale, 0.0f, 0.0f, 0.0f,
		0.0f, scale, 0.0f, 0.0f,
		0.0f, 0.0f, scale, 0.0f,
		animation[1].getPos().x,  animation[1].getPos().y,  animation[1].getPos().z,  1.0f };

	directionT = glm::translate(ModelMatrix, glm::vec3(0.0f, -4.0f, 0.0f));
	//directionT = ModelMatrix;

	rotationMatrix = glm::mat4{
		cos(animation[1].getRot().y), 0.0f, sin(animation[1].getRot().y), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(animation[1].getRot().y), 0.0f, cos(animation[1].getRot().y), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };


	directionModel = directionT * rotationMatrix;
	mvp = ProjectionMatrix * ViewMatrix * directionModel;
	if (animation[1].charge > 0)
	{
		glm::vec3 playerPos(animation[1].getPos());
		glm::vec3 dir(-sin(animation[1].getRot().y), 0.0f, cos(animation[1].getRot().y));
		crosshairPos[1] = glm::vec3(playerPos.x, playerPos.y - 4.0f, playerPos.z) + (dir * 43.0f);
		line2.setPos(lerp(playerPos, crosshairPos[1], animation[1].charge));

		mvp = ProjectionMatrix * ViewMatrix * glm::translate(identity, line2.getPos()); //* directionModel;
		line2.draw(iModelViewProjectionLoc, mvp);
	}

	ModelMatrix = ModelMatrix * rotationMatrix;

	mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
	if (animation[1].lives > 0)
	{
		if (!animation[1].isInvincible())
			animation[1].draw(iModelViewProjectionLoc, mvp, 0, 2490);

		ModelMatrix = glm::mat4{
			scale, 0.0f, 0.0f, 0.0f,
			0.0f, scale, 0.0f, 0.0f,
			0.0f, 0.0f, scale, 0.0f,
			animation[1].getPos().x,  animation[1].getPos().y,  animation[1].getPos().z,  1.0f };
		mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

		animation[1].draw(iModelViewProjectionLoc, mvp, 2490, 882);
	}




	//glBindTexture(GL_TEXTURE_2D, texture_handle[2]);
	//glBindVertexArray(uiVAO2[1]);
	//glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	//glDrawArrays(GL_TRIANGLES, 0, object[sphereSpot + 1].getVertex().size());

	// Draw the bomb
	for (int i = 0; i < 2; i++)
	{

		glm::mat4 mvp = ProjectionMatrix * ViewMatrix * glm::mat4(
			5.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 5.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 5.0f, 0.0f,
			animation[i].bomb.getPos().x, animation[i].bomb.getPos().y, animation[i].bomb.getPos().z, 1.0f);

		//std::cout << animation[i].bomb.getPos().x << " " << animation[i].bomb.getPos().y << " " << animation[i].bomb.getPos().z << std::endl;

		if (animation[i].lives > 0)
			animation[i].bomb.draw(iModelViewProjectionLoc, mvp);

	}
	//if (aabb.collisionAABB(boundingBoxes[0], boundingBoxes[2]))//??
	//{
	//	x_bomb -= 0.3f;
	//}


	for (int i = 0; i < 2; i++)
	{
		float time = 0;
		PlayerObject *player = &animation[i];
		if (player->charge > 0)
		{
			float increment = 0.1f;

			glm::vec3 temp = glm::normalize(player->direction);
			temp.y = 1.0f;
			player->bomb.launch(player->getPos(), temp, player->charge);
			while (player->bomb.isActive())
			{
				time += 0.017;
				player->bomb.update(0.017);

				if (player->bomb.checkCollision(&object[0]).status)
				{
					player->bomb.explode();
				}

				if (player->bomb.isExploding())
				{
					player->bomb.reset();
					//crosshairPos[i] = player->bomb.getPos();
				}

				int iModelViewProjectionLoc = glGetUniformLocation(spMain.getProgramID(), "modelViewProjectionMatrix");
				int sampler = glGetUniformLocation(spMain.getProgramID(), "gSampler");

				glUniform1i(sampler, 0);
				glActiveTexture(GL_TEXTURE0);
				if (time > increment /*&& increment < 0.6f*/)
				{
					makeMatricies();
					glm::mat4 ProjectionMatrix = getProjectionMatrix();
					glm::mat4 ViewMatrix = getViewMatrix();
					glm::mat4 mvp = ProjectionMatrix * ViewMatrix * glm::mat4(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						player->bomb.getPos().x, player->bomb.getPos().y, player->bomb.getPos().z, 1.0f);

					player->bomb.draw(iModelViewProjectionLoc, mvp);
					increment += 0.05f;

				}

			}
		}
	}

}



/* function void KeyboardCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is pressed
*/
void KeyboardCallbackFunction(unsigned char key, int x, int y)
{
	KEYBOARD_INPUT->SetActive(key, true);
}

/* function void KeyboardUpCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is lifted
*/
void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{
	KEYBOARD_INPUT->SetActive(key, false);
}



void characterInput(PlayerObject *player, controller conPlayer)
{
	if (conPlayer.conButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		player->reset();
	}

	if (player->lives > 0)
	{
		bool tempBool = false;
		glm::vec3 tempVel;
		tempVel = player->getVel();
		Coords stick = conPlayer.getLeftStick();
		glm::vec2 sNormalized = glm::normalize(glm::vec2(stick.x, stick.y));
		//std::cout << stick.x << "   " << stick.y << std::endl;
		float temp = 1;
		bool sprint = conPlayer.conButton(XINPUT_GAMEPAD_B);
		if (stick.y < -0.1 || stick.y > 0.1)
		{
			player->addPos(glm::vec3(dt * playerSpeed*stick.y, 0.0f, 0.0f));
			//	std::cout << "left" << std::endl;
			if (sprint)
			{
				if (player->getVel().x == 0.0f && player->frame > 1)
				{
					player->addVel(glm::vec3(sNormalized.y * playerSpeed * 3, 0.0f, 0.0f));
					tempBool = true;
				}
			}
		}
		if (stick.x < -0.1 || stick.x > 0.1)
		{
			player->addPos(glm::vec3(0.0f, 0.0f, dt * playerSpeed*stick.x));
			if (sprint)
			{
				if (player->getVel().z == 0.0f && player->frame > 1)
				{
					player->addVel(glm::vec3(0.0f, 0.0f, sNormalized.x * playerSpeed * 3));
					tempBool = true;
				}
			}
			//	std::cout << "up" << std::endl;
		}

		if (tempBool == true)
		{
			player->frame = 0;
		}
		stick = conPlayer.getRightStick();
		//std::cout << stick.x << "   " << stick.y << std::endl;

		if (stick.y > 0.1 || stick.y < -0.1 || stick.x > 0.1 || stick.x < -0.1)
		{
			player->direction = glm::vec3(-stick.y, 0.0f, -stick.x);
			float angle = atan2(-stick.y, stick.x) + 180 * degToRad;
			player->setRot(glm::vec3(0.0f, angle, 0.0f));
			//	std::cout << "left" << std::endl;

			if (player->charge == 0)
				player->charge = 0.15;

			if (player->charge < 1.0)
				player->charge += 0.015;
		}
		else if (player->charge > 0 && !player->bomb.isActive())
		{
			glm::vec3 temp;
			temp = glm::normalize(player->direction);
			temp.y = 1.0f;
			player->throwBomb(temp);
		}

		if (player->getVel() != glm::vec3(0.0f))
		{
			player->addVel(-player->getVel() * dt * 6.0f);
			if (player->frame >= 0.2)
			{
				player->setVel(glm::vec3(0.0f));
			}
		}
		player->frame += dt;

		//if (conPlayer.conButton(XINPUT_GAMEPAD_A) && player->checkCollision(&object[0]).status)
		//{
		//	player->addPos(glm::vec3(0.0f, 1.0f, 0.0f));
		//}


		//if (conPlayer.conButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) && !player->bomb.isActive())
		//{
		//	if (player->charge == 0)
		//		player->charge = 0.15;
		//
		//	if (player->charge < 1.0)
		//		player->charge += 0.015;
		//
		//}
		//if (player->charge > 0 && conPlayer.conButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) == false
		//	&& !player->bomb.isActive())
		//{
		//	glm::vec3 temp;
		//	temp = glm::normalize(player->direction);
		//	temp.y = 1.0f;
		//	player->throwBomb(temp);
		//	/*player->bomb.setVel(glm::vec3(direction* player->charge));
		//	player->bomb.setVel(glm::vec3(temp* player->charge));
		//	player->bombThrow = true;
		//	player->bomb.setPos(player->getPos());
		//	player->bombTimer += dt;
		//	player->charge = 0;*/
		//}

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
}

void processMenuInputs()
{

	if (KEYBOARD_INPUT->CheckPressEvent('m'))
	{
		inMenu = !inMenu;

		if (inMenu)
		{
			position = menuDefaultPos;
			currentAngles = menuDefaultAngle;
		}
		else
		{
			position = gameDefaultPos;
			currentAngles = gameDefaultAngle;
		}
	}
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

	if (KEYBOARD_INPUT->CheckPressEvent('3'))
		//	selected = false;
		if (KEYBOARD_INPUT->CheckPressEvent('4'))
			//	selected = true;

			if (KEYBOARD_INPUT->IsKeyDown('f'))
			{
				UI_pos.x -= playerSpeed;
			}
	if (KEYBOARD_INPUT->IsKeyDown('h'))
	{
		UI_pos.x += playerSpeed;
	}
	if (KEYBOARD_INPUT->IsKeyDown('t'))
	{
		UI_pos.y += playerSpeed;
	}
	if (KEYBOARD_INPUT->IsKeyDown('g'))
	{
		UI_pos.y -= playerSpeed;
	}
	if (KEYBOARD_INPUT->IsKeyDown('y'))
	{
		UI_pos.z += playerSpeed;
	}
	if (KEYBOARD_INPUT->IsKeyDown('r'))
	{
		animation[0].setPos(glm::vec3(0.0, 10.0, -5.0));
		animation[1].setPos(glm::vec3(0.0, 10.0, -5.0));
	}

	if (KEYBOARD_INPUT->CheckPressEvent('z'))
	{
		lock = !lock;
	}
	if (KEYBOARD_INPUT->CheckPressEvent('q') ||
		KEYBOARD_INPUT->CheckPressEvent(27))
	{
		glutLeaveMainLoop();
	}
	if (KEYBOARD_INPUT->CheckPressEvent('c'))
	{
		cameraLock = !cameraLock;
	}
	if (KEYBOARD_INPUT->CheckPressEvent('m'))
	{
		inMenu = !inMenu;

		if (inMenu)
		{
			position = menuDefaultPos;
			currentAngles = menuDefaultAngle;
		}
		else
		{
			position = gameDefaultPos;
			currentAngles = gameDefaultAngle;
		}
	}

	//if (KEYBOARD_INPUT->IsKeyDown('1'))
	//{
	//	initialFoV += 0.05f;
	//}
	//if (KEYBOARD_INPUT->IsKeyDown('2'))
	//{
	//	initialFoV -= 0.05f;
	//}

}

// Handles all events (collision, etc.)
void handleEvents(float dt)
{
	// Handle events
	if (lock == true)
	{
		glutWarpPointer(windowWidth / 2, windowHeight / 2);
	}

	// Collision
	for (int i = 0; i < 2; i++)
	{
		Collision check = animation[i].bomb.checkCollision(&object[0]);
		if (check.status == true)
		{
			std::cout << "Bomb collided with table" << std::endl;
			animation[i].bomb.explode();
			//animation[i].fastCollisionFix(check, dt);
		}

		check = animation[i].checkCollision(&object[0]);
		if (check.status == true)
		{
			//std::cout << "player collided with table" << std::endl;

			//std::cout << animation[i].getAcc().y << std::endl;
			//glm::vec3 normalForce(0.0f, -animation[i].getAcc().y, 0.0f);
			//animation[i].addForce(dt, normalForce);
			//animation[i].addAcc(normalForce);
			animation[i].useGravity(false);
			animation[i].setAcc(glm::vec3(0.0f));
			animation[i].setVel(animation[i].getVel() * glm::vec3(1.0f, 0.0f, 1.0f));
		}
	}

	Collision check = animation[0].bomb.checkCollision(&animation[1]);
	if (check.status == true && !animation[1].isInvincible())
	{
		animation[0].bomb.explode();
		animation[1].takeDamage(1);
	}

	check = animation[1].bomb.checkCollision(&animation[0]);
	if (check.status == true && !animation[0].isInvincible())
	{
		animation[1].bomb.explode();
		animation[0].takeDamage(1);
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

	// Process all inputs /////////////////////////////////////////////////////
	if (inMenu) processMenuInputs();
	else processInputs();
	// Flush event list
	KEYBOARD_INPUT->WipeEventList();

	//// update physics	///////////////////////////////////////////////////////
	static int elapsedTimeAtLastTick = 0;
	int totalElapsedTime = glutGet(GLUT_ELAPSED_TIME);

	dt = totalElapsedTime - elapsedTimeAtLastTick;
	dt /= 1000.0f;
	elapsedTimeAtLastTick = totalElapsedTime;

	// Update the objects /////////////////////////////////////////////////////
	for (GameObject & o : object)
	{
		o.update(dt);
	}

	for (PlayerObject & o : animation)
	{
		o.update(dt);
		o.bomb.update(dt);
	}

	//	handle all events /////////////////////////////////////////////////////
	handleEvents(dt);

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
	glutCreateWindow("window title");

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

	// Cleanup
	KEYBOARD_INPUT->Destroy();

	return 0;


}

