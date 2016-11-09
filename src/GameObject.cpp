#include "GameObject.h"
#include <IL\ilut.h>
#include "glm\gtc\type_ptr.hpp"
#include <stdexcept>

GameObject::GameObject()
{
	isEnvironment = false;
	textureUnit = 0;
	acc = glm::vec3(0.0f);
	vel = glm::vec3(0.0f);
}

GameObject::GameObject(char const* filePath)
{
	loadBaseObject(filePath);
	isEnvironment = false;
	textureUnit = 0;
	acc = glm::vec3(0.0f);
	vel = glm::vec3(0.0f);
}

GameObject::GameObject(char const* filePath, char * texData)
{
	loadBaseObject(filePath);
	bindTexture(texData);
	isEnvironment = false;
	textureUnit = 0;
	acc = glm::vec3(0.0f);
	vel = glm::vec3(0.0f);
}

void GameObject::loadBaseObject(char const* filePath)
{
	obj = Loader(filePath);
}

void GameObject::update(float deltaTime)
{
	vel += acc * deltaTime;
	pos += vel * deltaTime;
}

void GameObject::draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp)
{
	glBindTexture(GL_TEXTURE_2D, texHandle[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(uiVAO[0]);
	glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, 0, obj.getVertices().size());
}

void GameObject::draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp, int point1, int point2)
{
	glBindTexture(GL_TEXTURE_2D, texHandle[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(uiVAO[0]);
	glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, point1, point2);
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

	textureUnit = GL_TEXTURE0 + textureStart;
	textureStart++;

	texHandle[0] = ilutGLLoadImage(filePath);
	glBindTexture(GL_TEXTURE_2D, texHandle[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP),
		ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData()); /* Texture specification */
}

void GameObject::setPos(glm::vec3 const & _set) { pos = _set; };
void GameObject::setVel(glm::vec3 const & _set) { vel = _set; };
void GameObject::setAcc(glm::vec3 const & _set) { acc = _set; };
void GameObject::setRot(glm::vec3 const & _set) { rot = _set; };

void GameObject::addPos(glm::vec3 const & _set) { pos += _set; };
void GameObject::addVel(glm::vec3 const & _set) { vel += _set; };
void GameObject::addAcc(glm::vec3 const & _set) { acc += _set; };
void GameObject::addRot(glm::vec3 const & _set) { rot += _set; };

glm::vec3 const & GameObject::getPos() const { return  pos; };
glm::vec3 const & GameObject::getVel() const { return  vel; };
glm::vec3 const & GameObject::getAcc() const { return  acc; };
glm::vec3 const & GameObject::getRot() const { return  rot; };


//////////ANIMATED OBJECT

AnimatedObject::AnimatedObject()
	: GameObject()
{
	currentAnim = 0;
	addAnim(Animation());
}

AnimatedObject::AnimatedObject(const char* basePosePath)
	: GameObject(basePosePath)
{
	currentAnim = 0;
	setBaseAnim(basePosePath);
}

AnimatedObject::AnimatedObject(char const* basePosePath, char * texData)
	: GameObject(basePosePath, texData)
{
	currentAnim = 0;
	setBaseAnim(basePosePath);
}

void AnimatedObject::setBaseAnim(char const* basePosePath)
{
	// Set the base pose as the default animation
	std::vector<Loader> tempAnimation;
	tempAnimation.push_back(Loader(basePosePath));
	addAnim(Animation(tempAnimation));
}

void AnimatedObject::addAnim(Animation const& newAnim)
{
	currentAnim = 0;
	animations.push_back(newAnim);
}

void AnimatedObject::addAnim(const char* filePath)
{
	addAnim(Animation(filePath));
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
	//animations.at(currentAnim).update(deltaTime, getBaseLoader());
	GameObject::bindObjectData(GL_DYNAMIC_DRAW);
}

void AnimatedObject::setCurrentAnim(int newAnimIndex)
{
	if (newAnimIndex >= animations.size())
	{
		throw std::out_of_range("Error: given index out of range in AnimatedObject.");
	}

	currentAnim = newAnimIndex;
}

PlayerObject::PlayerObject(char const* basePosePath, char * texData, char const* bombPath, char * bombTex, int _temp)
	: AnimatedObject(basePosePath, texData), bomb(bombPath, bombTex)
{
	bombThrow = false;
	bombTimer = 0;
	score = 0;
	charge = 0;
	lives = 2;
	temp = _temp;
}

PlayerObject::PlayerObject()
{
	bombThrow = false;
	bombTimer = 0;
	score = 0;
	charge = 0;
	lives = 2;
	temp = NULL;

}