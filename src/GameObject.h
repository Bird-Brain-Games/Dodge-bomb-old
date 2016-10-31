// A class that contains all the shared attributes that every loaded object has.

#pragma once
#include "objLoader.h"
#include <glm\glm.hpp>

static int textureStart = 0;

class GameObject
{
public:
	GameObject();
	GameObject(char const* filePath);

	void loadObject(char const* filePath);

	void update(float deltaTime);
	void draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp);

	void bindObjectData(GLuint DrawType = GL_DYNAMIC_DRAW);
	void bindTexture(char* filePath);

	Loader const& getLoader() { return obj; }
	GLuint const* getVAO() { return uiVAO; }	// To be removed probably

private:
	Loader obj;
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 acc;
	glm::vec3 rot;

	GLuint uiVAO[1];
	GLuint uiVBO[3];
	GLuint texHandle[1];
	GLuint texSampler[1];

	int textureUnit;
};