#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <gl\glew.h>
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
	Loader(char const* filePath);

	bool load(char const* filePath);
	std::vector<glm::vec3> const& getVertices() const;
	std::vector<glm::vec2> const& getUVs() const;
	std::vector<glm::vec3> const& getNormals() const;
	std::vector<float> const& getColors() const;
	void setVertex(int index, glm::vec3 const& newVertex);
	void setVertices(std::vector<glm::vec3> const& newVertices);

private:
	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec2> out_uvs;
	std::vector<glm::vec3> out_normals;
	std::vector<float> color;
};

// Animation class used to control flow of objects and speed
class Animation
{
public:
	Animation();
	Animation(const char* filePath);
	Animation(std::vector<Loader> const&);
	
	void update(float deltaTime, Loader & base);
	void reset();

private:
	std::vector<Loader> poseList;
	int currentPose, nextPose;
	
	// To put speed control in here
	float time;
};