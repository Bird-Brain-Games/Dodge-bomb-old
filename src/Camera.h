#pragma once

#include "glm\common.hpp"
#include "glm\gtc\quaternion.hpp"

class Camera
{
public:
	Camera();
	glm::mat4x4 getLookAt();

	void setDest(glm::vec3);
	void update(float dt);

public:
	glm::vec3	pos;
	glm::vec3	destPos;
	glm::vec2	destAngles;
	//glm::quat	pos;
	//glm::quat	dest;
	glm::vec3	dir;
	glm::vec3	up;
	glm::vec3	right;
	glm::vec2	currentAngles;

};