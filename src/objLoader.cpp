#include "objLoader.h"
Loader::Loader(int x)
{
	data = x;
}

bool Loader::load(std::string name)
{
	std::string temp;
	std::string converter;
	int counter = 0;
	int startPos = 0;
	bool start = false;
	std::ifstream file(name += ".obj");
	int dataCounter;


	while (std::getline(file, temp))
	{
		if (temp.empty() == 0 && temp.at(0) == 'v' && temp.at(1) == ' ')
		{

			for (int count = 1; count <= temp.size() - 1; count++)
			{
				if ((temp.at(count) != ' ' && start == false))
				{
					startPos = count;
					start = true;
				}
				if (start == true && (temp.at(count) == ' ' || count == temp.size() - 1))
				{
					converter = temp.substr(startPos, count - startPos);
					vertexs.push_back(std::stof(converter));
					start = false;
				}

			}
		}
		if (temp.empty() == 0 && temp.at(0) == 'f')
		{
			for (int count = 1; count <= temp.size() - 1; count++)
			{
				if (temp.at(count) >= 48 && temp.at(count) <= 57)
				{
					counter = count+1;
					while (counter < temp.size() && temp.at(counter) >= 48 && temp.at(counter) <= 57)
					{
						counter++;
					}
					converter = temp.substr(count, counter - count);
					testing.push_back(std::stoi(converter));
					count = counter;
					counter = 0;
				}

			}
		}
	}
	file.close();
	//glGenBuffers(1, &uiBuffer);
	

	return true;
}
std::vector<int>& Loader::getOrder()
{
	return testing;
}               

std::vector<float>& Loader::getVertex()
{
	return vertexs;
}


                                                                                                                                                                                                                                                                                                                                       