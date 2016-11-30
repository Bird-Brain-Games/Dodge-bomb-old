#pragma once

#include "GameObject.h"

// Game object that supports multiple textures on an object
class MultiTexObject : public GameObject
{
public:
	//void draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp);
	MultiTexObject(char const* filePath, char * texData, glm::vec3 dimensions);

	void setTex(int newTex);
	void addTex(char *filePath);

private:
	std::vector<GLuint> texHandles;
	GLuint *currentHandle;
};

// Game object that contains several objects that you can switch between.
class MultiObject : public GameObject
{
public:
	MultiObject(char const* filePath, char * texData, glm::vec3 dimensions);

	void draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp);
	void update(float dt);

	void addObject(GameObject*);
	void setActive(int it);

private:
	std::vector<GameObject> objects;
	GameObject *currentObject;
};

enum MENU_STATE
{
	MAIN,
	READY,
	RESULTS
};

// Menu class contains options for menus and holds ready data
class Menu
{
public:
	Menu();

	void update(float dt);
	void draw();

private:
	bool p1Ready, p2Ready;
	MENU_STATE state;
	std::vector<MultiObject> characterReady;
	std::vector<MultiObject> health;
	std::vector<MultiObject> menuStates;
	std::vector<MultiObject> characterWin;
	//std::vec
};