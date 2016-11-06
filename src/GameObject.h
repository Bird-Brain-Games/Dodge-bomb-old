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
	GameObject(char const* filePath, char * texData);

	void loadBaseObject(char const* filePath);

	void update(float deltaTime);
	void draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp);

	void bindObjectData(GLuint DrawType = GL_DYNAMIC_DRAW);
	void bindTexture(char * filePath);

	Loader & getBaseLoader() { return obj; }
	GLuint const* getVAO() { return uiVAO; }	// To be removed probably

	//setters and getters
	void setPos(glm::vec3 const & _set);
	void setVel(glm::vec3 const & _set);
	void setAcc(glm::vec3 const & _set);
	void setRot(glm::vec3 const & _set);

	void addPos(glm::vec3 const & _set);
	void addVel(glm::vec3 const & _set);
	void addAcc(glm::vec3 const & _set);
	void addRot(glm::vec3 const & _set);

	glm::vec3 const & getPos() const ;
	glm::vec3 const & getVel() const ;
	glm::vec3 const & getAcc() const ;
	glm::vec3 const & getRot() const ;

protected:
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 acc;
	glm::vec3 rot;

private:
	Loader obj;
	bool isEnvironment;

	GLuint uiVAO[1];
	GLuint uiVBO[3];
	GLuint texHandle[1];
	GLuint texSampler[1];

	int textureUnit;
};

// Animated object takes a base pose, and holds several animations.
class AnimatedObject : public GameObject
{
public:
	AnimatedObject();
	AnimatedObject(char const* basePosePath);
	AnimatedObject(char const* basePosePath, char * texData);

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


class PlayerObject : public AnimatedObject
{
public:
	PlayerObject(char const* basePosePath, char * texData, char const* bombPath, char * bombTex, int _temp);
	PlayerObject();
	GameObject bomb;
	int temp;//temp variable to tell me what bounding box to use for the bomb. will be replaced when the bounding boxes are added to class.
	bool bombThrow;
	float bombTimer;
	int score;
	float charge;
	int lives;
	int controllerState;


};