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
	std::vector<glm::vec2>& getUV();
	std::vector<glm::vec3>& getNormal();
	std::vector<float>& getColor();
private:
	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec2> out_uvs;
	std::vector<glm::vec3> out_normals;
	std::vector<float> color;
	//std::vector<BYTE> data;
	UINT uiVBO;
	UINT uiVAO;
	UINT uiBuffer;
};