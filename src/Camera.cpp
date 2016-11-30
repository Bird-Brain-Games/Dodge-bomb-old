#include "Camera.h"
#include "math.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

void Camera::setDest(glm::vec3 _dest)
{
	//dest = glm::quat_cast(glm::translate(glm::mat4(1.0f));
}

void Camera::update(float dt)
{
	/*if (pos != dest)
	{

		
	}*/

	dir = glm::vec3(
		cos(currentAngles.y) * sin(currentAngles.x),
		sin(currentAngles.y),
		cos(currentAngles.y) * cos(currentAngles.x)
	);
	right = glm::vec3(
		sin(currentAngles.x - 3.14f / 2.0f),
		0,
		cos(currentAngles.x - 3.14f / 2.0f)
	);
	up = glm::cross(right, dir);
}