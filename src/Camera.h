#pragma once

#include "glm\common.hpp"

class Camera
{
public:
	Camera();
	glm::mat4x4 getLookAt();

private:
	glm::vec3	pos;
	glm::vec3	vel;
	glm::vec3	dir;
	glm::vec3	up;
	glm::vec3	right;

};