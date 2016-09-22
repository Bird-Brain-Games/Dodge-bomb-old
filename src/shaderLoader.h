#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

class ShaderLoader
{
public:
	ShaderLoader();
	bool loadShader(std::string fileName, int shaderType);
	UINT getID();
private:
	UINT shaderID;// Shader ID
	int sType; // what type of shader that the program will load
	bool loaded;// whether everthing worked

};