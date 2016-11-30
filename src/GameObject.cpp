#pragma once
#include "GameObject.h"
#include <IL\ilut.h>
#include "glm\gtc\type_ptr.hpp"
#include "GLM\gtx\projection.hpp"
#include "GLUT\glut.h"
#include <stdexcept>
#include "math.h"
#include "glm\gtx\transform.hpp"


GameObject::GameObject()
{
	isEnvironment = false;
	scale = glm::vec3(1.0f);
	pos = glm::vec3(0.0f);
	rot = glm::vec3(0.0f);
	dimension = glm::vec3(1.0f);
	setMass(1.0f);
	acc = glm::vec3(0.0f);
	vel = glm::vec3(0.0f);
}

GameObject::GameObject(char const* filePath)
{
	boundingBox = Loader("obj\\cube.obj");
	loadBaseObject(filePath);
	isEnvironment = false;
	scale = glm::vec3(1.0f);
	acc = glm::vec3(0.0f);
	vel = glm::vec3(0.0f);
	dimension = glm::vec3(1.0f);
	setMass(1.0f);
}

GameObject::GameObject(char const* filePath, char * texData, glm::vec3 _dimension)
{
	boundingBox = Loader("obj\\cube.obj");
	loadBaseObject(filePath);
	bindTexture(texData);
	isEnvironment = false;
	dimension = _dimension;
	acc = glm::vec3(0.0f);
	vel = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	setMass(1.0f);
}

void GameObject::loadBaseObject(char const* filePath)
{
	obj = Loader(filePath);
}

void GameObject::update(float deltaTime)
{
	this;
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
	return checkCollision(other->getPos(), other->dimension);
}

Collision GameObject::checkCollision(glm::vec3 otherPos, glm::vec3 otherDimensions)
{
	Collision check;

	float thisMaxX = getPos().x + dimension.x;	// getPos() - width / 2
	float thisMinX = getPos().x - dimension.x;
	float thisMaxY = getPos().y + dimension.y;
	float thisMinY = getPos().y - dimension.y;
	float thisMaxZ = getPos().z + dimension.z;
	float thisMinZ = getPos().z - dimension.z;

	float otherMaxX = otherPos.x + otherDimensions.x;	// getPos() - width / 2
	float otherMinX = otherPos.x - otherDimensions.x;
	float otherMaxY = otherPos.y + otherDimensions.y;
	float otherMinY = otherPos.y - otherDimensions.y;
	float otherMaxZ = otherPos.z + otherDimensions.z;
	float otherMinZ = otherPos.z - otherDimensions.z;

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
		check.normal = glm::normalize(otherPos - getPos());

	return check;
}

void GameObject::fastCollisionFix(Collision const& col, float deltaTime)
{
	glm::vec3 correction = -glm::proj(col.overlap, getVel());
	std::cout << "Correction: " << correction.x << ", " << correction.y << std::endl;
	addPos(-correction * deltaTime);
}

void GameObject::draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp, int point1, int point2)
{
	glBindTexture(GL_TEXTURE_2D, texHandle[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(uiVAO[0]);
	glUniformMatrix4fv(iModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, point1, point2);
}

void GameObject::draw(GLint *iModelViewProjectionLoc[], glm::mat4 const& mvp, int count, glm::mat4 matrix[], int point1, int point2)
{
	glBindTexture(GL_TEXTURE_2D, texHandle[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(uiVAO[0]);
	glUniformMatrix4fv(*iModelViewProjectionLoc[0], 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform1f(*iModelViewProjectionLoc[1], count);
	glUniformMatrix4fv(*iModelViewProjectionLoc[2], 28, GL_FALSE, glm::value_ptr(matrix[0]));
	glDrawArrays(GL_TRIANGLES, point1, point2);
}


void GameObject::bindObjectData(GLuint DrawType)
{
	glGenVertexArrays(2, uiVAO);
	glGenBuffers(5, uiVBO);

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
		glGenBuffers(3, uiVBOD);
		glBindVertexArray(uiVAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOD[0]);
		glBufferData(GL_ARRAY_BUFFER, boundingBox.getVertices().size() * sizeof(glm::vec3), boundingBox.getVertices().data(), DrawType);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, uiVBOD[1]);
		glBufferData(GL_ARRAY_BUFFER, boundingBox.getUVs().size() * sizeof(glm::vec2), boundingBox.getUVs().data(), DrawType);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, uiVBOD[2]);
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

//////////JOINT OBJECT /////////////////////////////////////////////////////

#include <iostream>

Joints::Joints()
	: m_pScale(1.0f),
	colour(glm::vec4(1.0)),
	m_pCurrentFrame(0),
	jointAnimation(nullptr)
{
	m_pRotX = 0;
	m_pRotY = 0;
	m_pRotZ = 0;
}

Joints* Joints::getParent()
{
	return m_pParent;
}

Joints::~Joints() {}

void Joints::setWorldMatrix(glm::mat4 temp)
{
	m_pLocalToWorldMatrix = temp;
}

void Joints::setPosition(glm::vec3 newPosition)
{
	m_pLocalPosition = newPosition;
}

void Joints::setRotationAngleX(float newAngle)
{
	m_pRotX = newAngle;
}

void Joints::setRotationAngleY(float newAngle)
{
	m_pRotY = newAngle;
}

void Joints::setRotationAngleZ(float newAngle)
{
	m_pRotZ = newAngle;
}

void Joints::setScale(float newScale)
{
	m_pScale = newScale;
}

glm::mat4 Joints::getLocalToWorldMatrix()
{
	return m_pLocalToWorldMatrix;
}

glm::mat4 Joints::getLocalTransform()
{
	return m_pLocalTransformMatrix;
}

void Joints::update(float dt)
{
	// Create 4x4 transformation matrix

	// If there is no animation for this joint, create the transform matrix as usual
	if (jointAnimation == nullptr)
	{
		// Create rotation matrix
		glm::mat4 rx = glm::rotate(glm::radians(m_pRotX), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 ry = glm::rotate(glm::radians(m_pRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rz = glm::rotate(glm::radians(m_pRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

		// Note: pay attention to rotation order, ZYX is not the same as XYZ
		// Order is specified by an HTR file when you export it
		m_pLocalRotation = rz * ry * rx;

	}
	else // Transform comes from HTR
	{
		m_pLocalRotation =
			glm::mat4_cast(jointAnimation->jointBaseRotation * jointAnimation->jointRotations[m_pCurrentFrame]);

		m_pLocalPosition = jointAnimation->jointBasePosition + jointAnimation->jointPositions[m_pCurrentFrame];

		jointAnimation->jointScales[m_pCurrentFrame];

		// Increment frame (note: you could do this based on dt)
		m_pCurrentFrame++;

		if (m_pCurrentFrame >= jointAnimation->numFrames)
			m_pCurrentFrame = 0;

	}

	// Create translation matrix
	glm::mat4 tran = glm::translate(m_pLocalPosition);

	// Create scale matrix
	glm::mat4 scal = glm::scale(glm::vec3(m_pScale, m_pScale, m_pScale));

	// Combine all above transforms into a single matrix
	// This is the local transformation matrix, ie. where is this game object relative to it's parent
	// If a game object has no parent (it is a root node) then its local transform is also it's global transform
	// If a game object has a transform, then we must apply the parent's transform
	m_pLocalTransformMatrix = tran * m_pLocalRotation * scal;

	if (m_pParent && m_pParent != NULL)
		m_pLocalToWorldMatrix = m_pParent->getLocalToWorldMatrix() * m_pLocalTransformMatrix;
	else
		m_pLocalToWorldMatrix = m_pLocalTransformMatrix;

	// Update children
	for (int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->update(dt);
}

void Joints::draw()
{
	// Draw a coordinate frame for the object
	glm::vec3 wPos = getWorldPosition();
	glm::mat4 wRot = getWorldRotation();

	glm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);

	//TTK::Graphics::DrawVector(wPos, wRot[0], 3.0f, red);
	//TTK::Graphics::DrawVector(wPos, wRot[1], 3.0f, green);
	//TTK::Graphics::DrawVector(wPos, wRot[2], 3.0f, blue);

	// Draw node
	//TTK::Graphics::DrawSphere(m_pLocalToWorldMatrix, 0.5f, colour);

	// Draw line to parent 
	if (m_pParent && !m_pParent->isRoot())
	{
		glm::vec3 parentWorldPosition = m_pParent->getWorldPosition();
		glm::vec3 myWorldPositon = getWorldPosition();

		//TTK::Graphics::DrawLine(myWorldPositon, parentWorldPosition, 5.0f);
	}

	// Draw children
	for (int i = 0; i < m_pChildren.size(); ++i)
		m_pChildren[i]->draw();

}

void Joints::setParent(Joints* newParent)
{
	m_pParent = newParent;
}

void Joints::addChild(Joints* newChild)
{
	if (newChild)
	{
		m_pChildren.push_back(newChild);
		newChild->setParent(this); // tell new child that this game object is its parent
	}
}

void Joints::removeChild(Joints* rip)
{
	for (int i = 0; i < m_pChildren.size(); ++i)
	{
		if (m_pChildren[i] == rip) // compare memory locations (pointers)
		{
			//	std::cout << "Removing child: " + rip->name << " from object: " << this->name;
			m_pChildren.erase(m_pChildren.begin() + i);
		}
	}
}

glm::vec3 Joints::getWorldPosition()
{
	if (m_pParent)
		return m_pParent->getLocalToWorldMatrix() * glm::vec4(m_pLocalPosition, 1.0f);
	else
		return m_pLocalPosition;
}

glm::mat4 Joints::getWorldRotation()
{
	if (m_pParent)
		return m_pParent->getWorldRotation() * m_pLocalRotation;
	else
		return m_pLocalRotation;
}

bool Joints::isRoot()
{
	if (m_pParent)
		return false;
	else
		return true;
}





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
	//animations.at(currentAnim).update(deltaTime, getBaseLoader());
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

PlayerObject::PlayerObject(char const* basePosePath, char * texData, int _side, glm::vec3 _dimension)
	: AnimatedObject(basePosePath, texData, _dimension),
	bomb("obj\\bomb.obj", "img\\bPileDiffuse.png", _side), weights("obj\\test.fbx")
{
	score = 0;
	charge = 0;
	lives = 2;
	side = _side;
	maxiFrames = 3.0f;
	currentiFrames = maxiFrames;

	//GameObject::obj.out_vertices = weights.vertexs;

	loader.loadHTR("obj\\bombot.htr");
	loader.createGameObjects();
	jointNum = 28;
	skeleton.addChild(loader.getRootGameObject());
}

PlayerObject::PlayerObject()
	: AnimatedObject(), weights("obj\\test.fbx")
{
	score = 0;
	charge = 0;
	lives = 2;
	side = 0;
	maxiFrames = 3.0f;
	currentiFrames = maxiFrames;

	loader.loadHTR("obj\\bombot.htr");
	loader.createGameObjects();
	jointNum = 28;
	skeleton.addChild(loader.getRootGameObject());


}

void PlayerObject::update(float dt)
{
	AnimatedObject::update(dt);

	//glm::mat4 trans = glm::translate(GameObject::getPos());
	//glm::mat4 X = glm::rotate(GameObject::getRot().x, glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::mat4 Y = glm::rotate(GameObject::getRot().y, glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 Z = glm::rotate(GameObject::getRot().z, glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 rot = Z * Y * Y;
	//glm::mat4 scale = glm::scale(GameObject::scale);
	//
	//glm::mat4 finals = trans * rot * scale;


	//loader.setWorldOrigin(finals);

	//skeleton.setPosition(GameObject::getPos());
	//skeleton.setParent(NULL);
	//skeleton.m_pChildren[0]->m_pParent = &skeleton;
	//skeleton.update(dt);//using frames not dt.
	if (currentiFrames < maxiFrames)
	{
		currentiFrames += dt;
		std::cout << currentiFrames << std::endl;
		if (currentiFrames > maxiFrames)
			currentiFrames = maxiFrames;
	}


	//for (int i = 0; i < 28; i++)
	//{
	//	glm::mat4 b = glm::inverse(loader.getJointList()[i].getLocalTransform());
	//	glm::mat4 w;
	//
	//	if (i == 0)
	//		w = glm::mat4();
	//	else
	//		w = loader.getJointList()[i].getParent()->getLocalToWorldMatrix();
	//	boneMatrix[i] = w * b;
	//}
	//int test = 5;
}

void PlayerObject::setSide(int _side)
{
	side = _side;
}

void PlayerObject::bindObjectData(GLuint DrawType)
{
	GameObject::bindObjectData(DrawType);
	bindskeleton();
	bomb.bindObjectData();
}

void PlayerObject::throwBomb(glm::vec3 direction)
{
	bomb.launch(getPos(), direction, charge);
	charge = 0.0f;
}

bool PlayerObject::isInvincible() const
{
	return (currentiFrames < maxiFrames);
}

void PlayerObject::takeDamage(int damage)
{
	currentiFrames = 0.0f;
	lives -= damage;
}

void PlayerObject::reset()
{
	lives = 2;
	setPos(glm::vec3(0.0f, 10.0f, 0.0f));
	useGravity(true);
}

void PlayerObject::bindskeleton(GLuint DrawType)
{
	glBindVertexArray(uiVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);

	glBufferData(GL_ARRAY_BUFFER, weights.getJoints().size() * sizeof(glm::vec4), weights.getJoints().data(), DrawType);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[4]);
	glBufferData(GL_ARRAY_BUFFER, weights.getWeights().size() * sizeof(glm::vec4), weights.getWeights().data(), DrawType);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void PlayerObject::draw(GLint* iModelViewProjectionLoc[], glm::mat4 const& mvp, int point1, int point2)
{
	//animation[0].draw(iModelViewProjectionLoc, mvp, animation[0].boneMatrix, animation[0].jointNum, 0, 2490);
	GameObject::draw(iModelViewProjectionLoc, mvp, jointNum, boneMatrix, point1, point2);
}

void PlayerObject::draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp, int point1, int point2)
{
	GameObject::draw(iModelViewProjectionLoc, mvp, point1, point2);
}

////////////////////////////////////////////////////////////  BOMB OBJECT  ////
Bomb::Bomb()
	: GameObject()
{
	reset();
}

Bomb::Bomb(char const* basePosePath, char * texData, int side)
	: GameObject(basePosePath, texData, glm::vec3(0.44f, 0.47f, 0.44f))
{
	reset();
	maxExplodeTimer = 0.5f;
	maxFuseTimer = 2.0f;

	glm::vec3 explDimension(3.0f);

	if (side == 0)
		explosion = GameObject("obj\\ball.obj", "img\\redTex.png", explDimension);
	else
		explosion = GameObject("obj\\ball.obj", "img\\blueTex.png", explDimension);

	explosion.bindObjectData();
}

void Bomb::update(float dt)
{
	GameObject::update(dt);

	// Increment the explosion timer
	if (isExploding())
	{
		currentExplodeTimer += dt;
		std::cout << currentExplodeTimer << std::endl;
		if (currentExplodeTimer >= maxExplodeTimer)
		{
			reset();
		}
		else
		{
			tVal = (currentExplodeTimer / maxExplodeTimer);
		}
	}
	else if (isActive())
	{
		currentFuseTimer += dt;
		if (currentFuseTimer >= maxFuseTimer)
		{
			explode();
			currentFuseTimer = 0.0f;
		}
	}
}

void Bomb::draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp)
{
	// if exploding, draw the solid sphere
	if (isExploding())
	{
		explosion.draw(iModelViewProjectionLoc, mvp);
	}
	else
	{
		GameObject::draw(iModelViewProjectionLoc, mvp);
	}
}

void Bomb::launch(glm::vec3 pos, glm::vec3 dir, float charge)
{
	setPos(pos);
	setVel(dir * charge * 65.0f);//* glm::vec3(0.0f, 5.0f, 0.0f));
	setVel(getVel() * glm::vec3(1.0f, 0.5f, 1.0f));
	//setAcc(glm::vec3(0.0f, 5.0f, 0.0f));
	active = true;
	useGravity(true);
}

Collision Bomb::checkCollision(GameObject * other)
{
	Collision col;
	if (isActive())
	{
		if (isExploding())
		{
			col = explosion.checkCollision(other);
		}
		else
		{
			col = GameObject::checkCollision(other);
		}
	}

	return col;
}

void Bomb::explode()
{
	if (!isExploding())
	{
		exploding = true;
		setVel(glm::vec3(0.0f));
		setAcc(glm::vec3(0.0f));
		explosion.setPos(getPos());
		useGravity(false);

		currentFuseTimer = 0.0f;
		currentExplodeTimer = 0.0f;
	}
}

void Bomb::reset()
{
	setPos(glm::vec3(0.0f, -100.0f, 0.0f));
	setVel(glm::vec3(0.0f));
	setAcc(glm::vec3(0.0f));

	useGravity(false);

	currentFuseTimer = 0.0f;
	currentExplodeTimer = 0.0f;
	tVal = 0.0f;

	exploding = false;
	active = false;
}

bool Bomb::isExploding() const { return exploding; }
bool Bomb::isActive() const { return active; }