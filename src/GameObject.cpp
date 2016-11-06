#include "GameObject.h"
#include <IL\ilut.h>
#include "glm\gtc\type_ptr.hpp"
#include "GLM\gtx\projection.hpp"
#include <stdexcept>
#include "math.h"

GameObject::GameObject()
{
	boundingBox = Loader("obj\\cube.obj");
	isEnvironment = false;
	scale = glm::vec3(1.0f);
	dimension = glm::vec3(1.0f);
	setMass(1.0f);
}

GameObject::GameObject(char const* filePath)
{
	boundingBox = Loader("obj\\cube.obj");
	loadBaseObject(filePath);
	isEnvironment = false;
	scale = glm::vec3(1.0f);
	dimension = glm::vec3(1.0f);
}

GameObject::GameObject(char const* filePath, char * texData, glm::vec3 _dimension)
{
	boundingBox = Loader("obj\\cube.obj");
	loadBaseObject(filePath);
	bindTexture(texData);
	isEnvironment = false;
	dimension = dimension;
	scale = glm::vec3(1.0f);
}

void GameObject::loadBaseObject(char const* filePath)
{
	obj = Loader(filePath);
}

void GameObject::update(float deltaTime)
{
	if (gravity)
	{
		addForce(deltaTime, GRAVITY);
	}

	setVel(getVel() + (getAcc() * deltaTime));
	setPos(getPos() + (getVel() * deltaTime));
}

void GameObject::draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp)
{
	glBindTexture(GL_TEXTURE_2D, texHandle[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(uiVAO[0]);
	glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, 0, obj.getVertices().size());

	// If debugging bounding, draw bounding boxes
	if (DEBUG_BOUNDING)
	{
		glBindVertexArray(uiVAO[1]);
		glm::mat4 newMvp = mvp * glm::mat4(
			dimension.x * 2.0f, 0.0f, 0.0f, 0.0f,
			0.0f, dimension.y * 2.0f, 0.0f, 0.0f,
			0.0f, 0.0f, dimension.z * 2.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
		glDrawArrays(GL_TRIANGLES, 0, obj.getVertices().size());
	}
}

void GameObject::addForce(float dt, glm::vec3 const& force, ForceMode mode)
{
	switch (mode)
	{
	case Force:
		addAcc(mass * force / pow(dt, 2.0f));
		break;
	case Acceleration:
		addAcc(force / pow(dt, 2.0f));
		break;
	case Impulse:
		addAcc(mass * force / dt);
		break;
	case VelocityChange:
		addAcc(force / dt);
		break;
	default:
		break;
	}
}

void GameObject::setScale(glm::vec3 newScale, bool changeBoundingBox)
{
	glm::vec3 oldScale = scale;
	scale = newScale;
	if (changeBoundingBox)
	{
		dimension *= scale;
	}

}

Collision GameObject::checkCollision(GameObject * other)
{
	Collision check;

	float thisMaxX = getPos().x + dimension.x;	// getPos() - width / 2
	float thisMinX = getPos().x - dimension.x;
	float thisMaxY = getPos().y + dimension.y;
	float thisMinY = getPos().y - dimension.y;
	float thisMaxZ = getPos().z + dimension.z;
	float thisMinZ = getPos().z - dimension.z;

	float otherMaxX = other->getPos().x + other->dimension.x;	// getPos() - width / 2
	float otherMinX = other->getPos().x - other->dimension.x;
	float otherMaxY = other->getPos().y + other->dimension.y;
	float otherMinY = other->getPos().y - other->dimension.y;
	float otherMaxZ = other->getPos().z + other->dimension.z;
	float otherMinZ = other->getPos().z - other->dimension.z;

	check.overlap.x = thisMaxX - otherMinX;
	check.overlap.y = thisMaxY - otherMinY;
	check.overlap.z = thisMaxZ - otherMinZ;

	check.status = (
		(thisMinX <= otherMaxX && thisMaxX >= otherMinX) &&
		(thisMinY <= otherMaxY && thisMaxY >= otherMinY) &&
		(thisMinZ <= otherMaxZ && thisMaxZ >= otherMinZ));

	if (getVel() == glm::vec3(0.0f))	// if both stationary, we have a problem
		check.normal = glm::vec3(0.0f, 0.0f, 0.0f);
	else
		check.normal = glm::normalize(other->getPos() - getPos());

	return check;
}

void GameObject::fastCollisionFix(Collision const& col, float deltaTime)
{
	glm::vec3 correction = -glm::proj(col.overlap, getVel());
	std::cout << "Correction: " << correction.x << ", " << correction.y << std::endl;
	addPos(-correction * deltaTime);
}

void GameObject::bindObjectData(GLuint DrawType)
{
	glGenVertexArrays(2, uiVAO);
	glGenBuffers(6, uiVBO);

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


	if (DEBUG_BOUNDING)
	{
		glBindVertexArray(uiVAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
		glBufferData(GL_ARRAY_BUFFER, boundingBox.getVertices().size() * sizeof(glm::vec3), boundingBox.getVertices().data(), DrawType);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, uiVBO[4]);
		glBufferData(GL_ARRAY_BUFFER, boundingBox.getUVs().size() * sizeof(glm::vec2), boundingBox.getUVs().data(), DrawType);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, uiVBO[5]);
		glBufferData(GL_ARRAY_BUFFER, boundingBox.getNormals().size() * sizeof(glm::vec3), boundingBox.getNormals().data(), DrawType);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
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

	glBindTexture(GL_TEXTURE_2D, 0);
}

void GameObject::setPos(glm::vec3 const & _set) { pos = _set; };
void GameObject::setVel(glm::vec3 const & _set) { vel = _set; };
void GameObject::setAcc(glm::vec3 const & _set) { acc = _set; };
void GameObject::setRot(glm::vec3 const & _set) { rot = _set; };
void GameObject::setMass(float _mass) { mass = _mass; }
void GameObject::useGravity(bool _gravity) { gravity = _gravity; }

void GameObject::addPos(glm::vec3 const & _set) { pos += _set; };
void GameObject::addVel(glm::vec3 const & _set) { vel += _set; };
void GameObject::addAcc(glm::vec3 const & _set) { acc += _set; };
void GameObject::addRot(glm::vec3 const & _set) { rot += _set; };

glm::vec3 const & GameObject::getPos() const { return  pos; };
glm::vec3 const & GameObject::getVel() const { return  vel; };
glm::vec3 const & GameObject::getAcc() const { return  acc; };
glm::vec3 const & GameObject::getRot() const { return  rot; };







//////////ANIMATED OBJECT /////////////////////////////////////////////////////

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

AnimatedObject::AnimatedObject(char const* basePosePath, char * texData, glm::vec3 _dimension)
	: GameObject(basePosePath, texData, _dimension)
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
	animations[currentAnim].update(deltaTime, getBaseLoader());
	//GameObject::bindObjectData(GL_DYNAMIC_DRAW);
}

void AnimatedObject::setCurrentAnim(int newAnimIndex)
{
	if (newAnimIndex >= animations.size())
	{
		throw std::out_of_range("Error: given index out of range in AnimatedObject.");
	}

	currentAnim = newAnimIndex;
}

PlayerObject::PlayerObject(char const* basePosePath, char * texData, glm::vec3 _dimension)
	: AnimatedObject(basePosePath, texData, _dimension),
	bomb("obj\\bomb.obj", "img\\bPileDiffuse.png")
{
	bombThrow = false;
	bombTimer = 0;
	score = 0;
	charge = 0;
	lives = 2;
}

PlayerObject::PlayerObject()
	: AnimatedObject()
{
	bombThrow = false;
	bombTimer = 0;
	score = 0;
	charge = 0;
	lives = 2;
}


Bomb::Bomb()
	: GameObject()
{

}

Bomb::Bomb(char const* basePosePath, char * texData)
	: GameObject(basePosePath, texData, glm::vec3(0.44f, 0.47f, 0.44f))
{

}