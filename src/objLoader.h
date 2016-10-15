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
	Loader();
	Loader(std::string);
	bool load(std::string);
	std::vector<glm::vec3> const& getVertex() const;
	std::vector<glm::vec2> const& getUV() const;
	std::vector<glm::vec3> const& getNormal() const;
	std::vector<float> const& getColor() const;

	void setVertex(int index, glm::vec3 newVertex);
private:
	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec2> out_uvs;
	std::vector<glm::vec3> out_normals;
	std::vector<float> color;
	std::vector<BYTE> data;
	UINT uiVBO;
	UINT uiVAO;
	UINT uiBuffer;
};