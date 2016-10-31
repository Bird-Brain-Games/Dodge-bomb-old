#include "GameObject.h"
#include <IL\ilut.h>
#include "glm\gtc\type_ptr.hpp"
#include <stdexcept>

GameObject::GameObject()
{
	isEnvironment = false;
}

GameObject::GameObject(char const* filePath)
{
	loadBaseObject(filePath);
	isEnvironment = false;
}

void GameObject::loadBaseObject(char const* filePath)
{
	obj = Loader(filePath);
}

void GameObject::update(float deltaTime)
{

}

void GameObject::draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp)
{
	glBindTexture(GL_TEXTURE_2D, texHandle[0]);
	glBindVertexArray(uiVAO[0]);
	glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, 0, obj.getVertices().size());
}

void GameObject::bindObjectData(GLuint DrawType)
{
	glGenVertexArrays(1, uiVAO);
	glGenBuffers(3, uiVBO);

	glBindVertexArray(uiVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, obj.getVertices().size() * sizeof(glm::vec3), obj.getVertices().data(), DrawType);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, obj.getUVs().size() * sizeof(glm::vec2), obj.getUVs().data(), DrawType);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, obj.getNormals().size() * sizeof(glm::vec3), obj.getNormals().data(), DrawType);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

}

void GameObject::bindTexture(char* filePath)
{
	glGenTextures(1, &texHandle[0]);
	glGenSamplers(1, &texSampler[0]);

	texHandle[0] = ilutGLLoadImage(filePath);
	glBindTexture(GL_TEXTURE_2D, texHandle[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP),
		ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData()); /* Texture specification */
}


AnimatedObject::AnimatedObject()
	: GameObject()
{

}

AnimatedObject::AnimatedObject(const char* filePath)
	: GameObject(filePath)
{

}

void AnimatedObject::addAnim(Animation const& newAnim)
{
	animations.push_back(newAnim);
}

void AnimatedObject::addAnim(const char* filePath)
{
	animations.push_back(Animation(filePath));
}

Animation const& AnimatedObject::getAnim(int pose) const
{
	try
	{
		return animations.at(pose);
	}
	catch (const std::out_of_range oor)
	{
		std::cerr << "Error: pose out of range." << std::endl;
	}

	return Animation();
}

void AnimatedObject::update(float deltaTime)
{
	GameObject::update(deltaTime);
	animations.at(currentAnim).update(deltaTime, getBaseLoader());
}

void AnimatedObject::setCurrentAnim(int newAnimIndex)
{

}