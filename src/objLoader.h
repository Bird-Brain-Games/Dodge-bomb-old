#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <glm\glm.hpp>

class uv
{
public:
	float u, v;
};

class Loader
{
public:
	bool load(std::string);
	std::vector<glm::vec3>& getVertex();
	std::vector<uv>& getUV();
	std::vector<glm::vec3>& getNormal();
	std::vector<glm::vec3>& getColor();
private:
	std::vector<glm::vec3> out_vertices;
	std::vector<uv> out_uvs;
	std::vector<glm::vec3> out_normals;
	std::vector<glm::vec3> color;

	UINT uiVBO;
	UINT uiVAO;
	UINT uiBuffer;
};