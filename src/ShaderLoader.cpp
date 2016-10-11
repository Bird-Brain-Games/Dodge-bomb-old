#include "shaderLoader.h"
#include "gl\glew.h"

ShaderLoader::ShaderLoader()
{
	loaded = false;
}

UINT ShaderLoader::getShaderID()
{
	return shaderID;
}

bool ShaderLoader::loadShader(std::string name, int shaderType)
{
	std::ifstream file(name);
	std::string temp;
	std::vector<std::string> lines;
	while (std::getline(file, temp))
	{
		lines.push_back(temp);
	}
	file.close();
	const char** size = new const char*[(int)lines.size()];
	for (int count = 0; count < (int)lines.size(); count++)
	{
		size[count] = lines[count].c_str();
	}
	glShaderSource(shaderType, lines.size(), size, NULL);
	glReleaseShaderCompiler();

	delete[] size;

	shaderID = shaderType;
	loaded = true;

	int iCompilationStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &iCompilationStatus);

	if (iCompilationStatus == GL_FALSE)return false;
	sType = shaderType;
	loaded = true;

	return true;
}

//FILE* fp = fopen(sFile.c_str(), "rt");
//if (!fp)return false;
//
//// Get all lines from a file
//
//vector<string> sLines;
//char sLine[255];
//while (fgets(sLine, 255, fp))sLines.push_back(sLine);
//fclose(fp);
//
//const char** sProgram = new const char*[ESZ(sLines)];
//FOR(i, ESZ(sLines))sProgram[i] = sLines[i].c_str();
//
//uiShader = glCreateShader(a_iType);
//
//glShaderSource(uiShader, ESZ(sLines), sProgram, NULL);
//glCompileShader(uiShader);
//
//delete[] sProgram;
//
//int iCompilationStatus;
//glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iCompilationStatus);
//
//if (iCompilationStatus == GL_FALSE)return false;
//iType = a_iType;
//bLoaded = true;
//
//return 1;