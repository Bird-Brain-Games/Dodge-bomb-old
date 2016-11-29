// A class that contains all the shared attributes that every loaded object has.

#pragma once

#include "objLoader.h"
#include <glm\glm.hpp>

enum ForceMode
{
	Force = 0,		// adds continuous force, uses mass
	Acceleration,	// adds continuous force, ignores mass
	Impulse,		// adds instant impulse, uses mass
	VelocityChange	// adds instant velocity change, ignores mass
};

struct Collision
{
	bool status;
	glm::vec3 overlap;
	glm::vec3 normal;
};

class GameObject
{
public:
	GameObject();
	GameObject(char const* filePath);
	GameObject(char const* filePath, char * texData, glm::vec3 dimensions);

	void loadBaseObject(char const* filePath);

	void update(float deltaTime);
	void draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp);
	void draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp, int point1, int point2);


	void bindObjectData(GLuint DrawType = GL_DYNAMIC_DRAW);
	void bindTexture(char * filePath);

	Loader & getBaseLoader() { return obj; }
	GLuint const* getVAO() { return uiVAO; }	// To be removed probably

	// Physics
	void addForce(float dt, glm::vec3 const& force, ForceMode mode = ForceMode::Force);

	// Collision
	Collision checkCollision(GameObject * other);
	Collision checkCollision(glm::vec3 otherPos, glm::vec3 otherDimensions);
	void fastCollisionFix(Collision const& col, float deltaTime);

	//setters and getters
	void setPos(glm::vec3 const & _set);
	void setVel(glm::vec3 const & _set);
	void setAcc(glm::vec3 const & _set);
	void setRot(glm::vec3 const & _set);
	void setMass(float _mass);
	void setScale(glm::vec3 newScale, bool changeBoundingBox = true);

	void addPos(glm::vec3 const & _set);
	void addVel(glm::vec3 const & _set);
	void addAcc(glm::vec3 const & _set);
	void addRot(glm::vec3 const & _set);

	glm::vec3 const & getPos() const;
	glm::vec3 const & getVel() const;
	glm::vec3 const & getAcc() const;
	glm::vec3 const & getRot() const;
	glm::vec3 const & getScale() const;

	void useGravity(bool _gravity = true);

protected:
	// Physics variables
	float mass;
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 acc;
	glm::vec3 rot;

	glm::vec3 dimension;
	glm::vec3 scale;

	bool gravity = false;

private:
	Loader obj;
	Loader boundingBox;
	bool isEnvironment;

	GLuint uiVAO[2];
	GLuint uiVBO[6];
	GLuint texHandle[1];
	GLuint texSampler[1];
};

// Animated object takes a base pose, and holds several animations.
class AnimatedObject : public GameObject
{
public:
	AnimatedObject();
	AnimatedObject(char const* basePosePath);
	AnimatedObject(char const* basePosePath, char * texData, glm::vec3 _dimension);

	// Add an animation to the list 
	// either by a file containing several poses,
	// or a premade animation object.
	void addAnim(char const* filePath);
	void addAnim(Animation const&);

	Animation const& getAnim(int index) const;
	void setCurrentAnim(int newAnimIndex);

	void update(float deltaTime);

protected:
	// Set the base pose as the default animation
	void setBaseAnim(char const* basePosePath);

protected:
	std::vector<Animation> animations;
	int currentAnim;
};

// Bomb class 
class Bomb : public GameObject
{
public:
	Bomb();
	Bomb(char const* basePosePath, char * texData, int side);

	void update(float dt);
	void draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp);
	void launch(glm::vec3 position, glm::vec3 direction, float charge);
	void explode();
	void reset();

	Collision checkCollision(GameObject * other);

	bool isExploding() const;
	bool isActive() const;

private:
	bool active;
	bool exploding;
	float currentFuseTimer;
	float maxFuseTimer;
	float currentExplodeTimer;
	float maxExplodeTimer;
	float tVal;

	// Explosion variables
	GameObject explosion;
};


class PlayerObject : public AnimatedObject
{
public:
	PlayerObject(char const* basePosePath, char * texData, int _side, glm::vec3 _dimension);
	PlayerObject();

	void update(float dt);

	void bindObjectData(GLuint DrawType = GL_DYNAMIC_DRAW);

	void throwBomb(glm::vec3 direction);

	void setSide(int _side);

	bool isInvincible() const;

	void takeDamage(int damage);

	// Debug command for resetting character
	void reset();

	Bomb bomb;
	int score;
	float charge;
	int lives;
	int controllerState;
	int side;

	float maxiFrames;
	float currentiFrames;

	glm::vec3 direction;

	float frame = 0;

};