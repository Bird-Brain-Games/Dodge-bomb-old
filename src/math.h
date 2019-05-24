#pragma once
#include "glm\glm.hpp"

glm::vec3 const GRAVITY(0.0f, -0.006f, 0.0f);
bool const DEBUG_BOUNDING = false;

struct boundingBox
{
	glm::vec3 max;
	glm::vec3 min;
};




template <typename T>
T lerp(T &p0, T &p1, float t)
{
	return (1.0f - t) * p0 + p1 * t;
}

template <typename T>
void lerp(T &p0, T &p1, T &pe, float t)
{
	for (int count = 0; count < pe.size(); count++)
	{
	pe[count] = (1.0f - t) * p0[count] + p1[count] * t; 
	}
}
