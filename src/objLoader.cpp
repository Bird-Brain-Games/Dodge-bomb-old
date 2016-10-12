#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "objLoader.h"
bool Loader::load(std::string fileName)
{
	FILE *file = fopen(fileName.c_str(), "r");//second parameter specifies what were doing with file ie read
	if (file == NULL)
	{
		printf("you didn't open a file");
		return false;
	}
	int lineCount = 0;


	std::vector<unsigned int> vertexIndecies, uvIndecies, normalindiecies;
	std::vector<glm::vec3>	temporary_vertices;
	std::vector<glm::vec2>	temporary_uvs;
	std::vector<glm::vec3>	temporary_normals;
	while (1)
	{
		char line[255];
		//read first word of the line
		int res = fscanf(file, "%s", line);
		if (res == EOF)
		{
			break;
		}

		if (strcmp(line, "vt") == 0)
		{
			glm::vec2 textureCoord;
			fscanf(file, "%f %f\n", &textureCoord.x, &textureCoord.y);
			temporary_uvs.push_back(textureCoord);
		}
		else if (strcmp(line, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temporary_normals.push_back(normal);
		}
		else if (strcmp(line, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temporary_vertices.push_back(vertex);

		}
		else if (strcmp(line, "f") == 0)
		{
			std::cout << strcmp(line, "f") << std::endl;
			unsigned int vertexIndex[3], UVindex[3], normalIndex[3];

			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &UVindex[0], &normalIndex[0],
				&vertexIndex[1], &UVindex[1], &normalIndex[1],
				&vertexIndex[2], &UVindex[2], &normalIndex[2]);
			vertexIndecies.push_back(vertexIndex[0]);
			vertexIndecies.push_back(vertexIndex[1]);
			vertexIndecies.push_back(vertexIndex[2]);

			//vboModelData.addData(&vVertices[iVertIndex - 1], sizeof(glm::vec3));
			//data.insert(data.end(), (BYTE*)ptrData, (BYTE*)ptrData + uiDataSize);

			uvIndecies.push_back(UVindex[0]);
			uvIndecies.push_back(UVindex[1]);
			uvIndecies.push_back(UVindex[2]);

			normalindiecies.push_back(normalIndex[0]);
			normalindiecies.push_back(normalIndex[1]);
			normalindiecies.push_back(normalIndex[2]);

			//for (int count = 0; count < 3; count++)
			//{
			//data.insert(data.end(), (BYTE*)&temporary_vertices[vertexIndex[count] - 1], sizeof(glm::vec3) + (BYTE*)&temporary_vertices[vertexIndex[count] - 1]);
			//data.insert(data.end(), (BYTE*)&temporary_uvs[UVindex[count] - 1], sizeof(glm::vec2) + (BYTE*)&temporary_vertices[UVindex[count] - 1]);
			//data.insert(data.end(), (BYTE*)&temporary_normals[normalIndex[count] - 1], sizeof(glm::vec3) + (BYTE*)&temporary_vertices[normalIndex[count] - 1]);
			//}
		}

	}
	//for each vertex of each triangle 
	for (unsigned int i = 0; i < vertexIndecies.size(); i++)
	{
		unsigned int vertexIndex = vertexIndecies[i];
		
		glm::vec3 vertex = temporary_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
		switch (i % 3)
		{
		case 0:
			color.push_back(1.0f);
			color.push_back(0.0f);
			color.push_back(0.0f);
			break;
		case 1:
			color.push_back(0.0f);
			color.push_back(1.0f);
			color.push_back(0.0f);
			break;
		case 2:
			color.push_back(0.0f);
			color.push_back(0.0f);
			color.push_back(1.0f);
			break;
		}
	}
	
	for (unsigned int i = 0; i < uvIndecies.size(); i++)
	{
		unsigned int uvIndex = uvIndecies[i];
		glm::vec2 uvs = temporary_uvs[uvIndex - 1];
		out_uvs.push_back(uvs);
	}
	
	for (unsigned int i = 0; i < normalindiecies.size(); i++)
	{
		unsigned int normalIndex = normalindiecies[i];
		glm::vec3 normal = temporary_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
	return true;

}

std::vector<glm::vec3>& Loader::getVertex() { return out_vertices; }
std::vector<glm::vec2>& Loader::getUV() { return out_uvs; }
std::vector<glm::vec3>& Loader::getNormal() { return out_normals; }
std::vector<float>& Loader::getColor() { return color; }