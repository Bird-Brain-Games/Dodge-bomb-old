// A class that contains all the shared attributes that every loaded object has.

#pragma once
#include "objLoader.h"
#include <glm\glm.hpp>

class GameObject
{
public:
	GameObject();
	GameObject(char const* filePath);

	void loadObject(char const* filePath);

	void update(float deltaTime);
	void draw();

private:
	Loader obj;
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 acc;
	glm::vec3 rot;
};