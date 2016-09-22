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
	//std::vector<float>& testing();
private:
	std::vector<float> vertexs;
	std::vector<int> testing;
	int data;

	UINT uiVBO;
	UINT uiVAO;
	UINT uiBuffer;
};