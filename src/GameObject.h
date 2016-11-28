// A class that contains all the shared attributes that every loaded object has.

#pragma once

#include "objLoader.h"
#include "glm\glm.hpp"
#include "fbxLoader.h"
#include "JointTypes.h"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"

class HTRLoader;

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

class Joints
{
protected:
	float m_pScale;

	float m_pRotX, m_pRotY, m_pRotZ; // local rotation angles

	glm::vec3 m_pLocalPosition;
	glm::mat4 m_pLocalRotation;

	glm::mat4 m_pLocalTransformMatrix;
	glm::mat4 m_pLocalToWorldMatrix;

	// Forward Kinematics
	Joints* m_pParent;
	std::vector<Joints*> m_pChildren;

	// HTR animation
	unsigned int m_pCurrentFrame;


public:
	Joints();
	Joints(std::string _name, glm::vec3 position);
	~Joints();

	void setPosition(glm::vec3 newPosition);
	void setRotationAngleX(float newAngle);
	void setRotationAngleY(float newAngle);
	void setRotationAngleZ(float newAngle);
	void setScale(float newScale);

	glm::mat4 getLocalToWorldMatrix();

	virtual void update(float dt);
	virtual void draw();

	// Animation from HTR
	JointDescriptor* jointAnimation;


	// Forward Kinematics
	// Pass in null to make game object a root node
	void setParent(Joints* newParent);
	void addChild(Joints* newChild);
	void removeChild(Joints* rip);
	glm::vec3 getWorldPosition();
	glm::mat4 getWorldRotation();
	bool isRoot();

	// Other Properties
	std::string name;
	glm::vec4 colour;
	// Mesh* ...
	// Material* ...
};

#pragma once

#include <string>
#include "JointTypes.h"

class HTRLoader
{
public:
	HTRLoader();

	// Loads the specified HTR file. 
	// Returns true if successful
	bool loadHTR(std::string fileName);

	// Loop through each joint descriptor until the input jointName is found
	// If not found, will return a nullptr
	JointDescriptor* getJointDescriptorByName(std::string jointName);

	// Returns pointer to specific joint
	Joints* getGameObjectByName(std::string jointName);

	// Turns the HTR file into usable game objects
	void createGameObjects();

	// Returns root node (usually the hip if a humanoid skeleton)
	Joints* getRootGameObject();

private:

	// Functions used in parsing process
	bool processHeaderSection(FILE* fp, char* loc);
	bool processHeader(std::string header, std::string value);
	bool processSegementNameSection(FILE* fp, char* loc);
	bool processBasePositionSection(FILE* fp, char* loc);
	bool processAnimationSection(FILE* fp, char* loc);

	// Increments the file pointer to the next line in the file
	// and copies it into buffer
	void goToNextValidLineInFile(FILE* fp, char* buffer);

	// Takes in 3 euler angles and returns a quaternion
	glm::quat createQuaternion(float rx, float ry, float rz);

	// Describes the data in the file

	std::string fileType;		// File extension
	std::string dataType;		// What kind of data is stored in file. 
								// Ie. HTRS means Hierarchical translations followed by rotations and scale

	int			fileVersion;	// Useful if you are generating binary object files on load, can check the version of an existing binary file, 
								// check version of text, if text version > binary version then reparse, otherwise just use binary

	int			numSegments;	// Number of bones in skeleton
	int			numFrames;		// number of frames in the animation
	int			fps;			// FPS of the animation

	RotationOrder rotationOrder;	// Order to apply rotations
	std::string	calibrationUnits;// 
	char		upAxis;			// X, Y, Z (usually Y)
	std::string rotationUnits;	// "Degrees" or "Radians"
	char		boneLengthAxis;	// Axis that is aligned with bone

								// Actual animation data
	std::vector<JointDescriptor> jointDescriptors;	// Stores each joint and its parent (could be an array since we know number of segments)

	std::vector<Joints> jointGameObjects;

	Joints* rootGameObject;
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

	HTRLoader loader;
	fbxLoader weights;
	Joints skeleton;
};