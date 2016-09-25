#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
class Loader
{
public:
	bool load(std::string name);
	Loader(int);
	std::vector<int>& getOrder();
	std::vector<float>& getVertex();
	void order();
	std::vector<float>& getVertexOrder();
	std::vector<float>& getColorOrder();
private:
	std::vector<float> vertexs;
	std::vector<int> testing;
	std::vector<float> vertexOrder;
	std::vector<float> colorOrder;
	int data;

	UINT uiVBO;
	UINT uiVAO;
	UINT uiBuffer;
};