#pragma once
#include "glm\glm.hpp"
struct boundingBox
{
	glm::vec3 max;
	glm::vec3 min;
};

template <typename T>
T lerp(T p0, T p1, float t)
{
	return (1.0f - t) * p0 + p1 * t;
}

class Collision
{
public:
	bool collisionAABB(boundingBox, boundingBox, bool);
	glm::vec3 getResult();
private:
	glm::vec3 result;
};


bool Collision::collisionAABB(boundingBox p0, boundingBox p1, bool _calc = false)
{
	if ((p0.min.x > p1.min.x && p0.min.x < p1.max.x) || (p0.max.x > p1.min.x && p0.max.x < p1.max.x))
		if ((p0.min.y > p1.min.y && p0.min.y < p1.max.y) || (p0.max.y > p1.min.y && p0.max.y < p1.max.y))
			if ((p0.min.z > p1.min.z && p0.min.z < p1.max.z) || (p0.max.z > p1.min.z && p0.max.z < p1.max.z))
				return true;

	return false;
}

glm::vec3 Collision::getResult()
{
	return result;
}