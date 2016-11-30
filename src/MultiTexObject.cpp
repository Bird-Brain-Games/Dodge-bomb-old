#include "MultiTexObject.h"
#include "glm\gtc\type_ptr.hpp"
#include <exception>

MultiTexObject::MultiTexObject(char const* filePath, char * texData, glm::vec3 dimensions)
	: GameObject(filePath, texData, dimensions)
{
	addTex(texData);
	currentHandle = &texHandles.at(0);
}
//
//void MultiTexObject::update(float dt)
//{
//
//}

//void MultiTexObject::draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp)
//{
//	
//
//}

void MultiTexObject::setTex(int newTex)
{
	try
	{
		currentHandle = &texHandles.at(newTex);
		texHandle[0] = *currentHandle;
	}
	catch (std::out_of_range err)
	{
		std::cerr << err.what() << std::endl;
	}
}

void MultiTexObject::addTex(char *filePath)
{
	GLuint newTex = bindTexture(filePath);
	texHandles.push_back(newTex);
}

MultiObject::MultiObject(char const* filePath, char * texData, glm::vec3 dimensions)
	: GameObject()
{
	addObject(&GameObject(filePath, texData, dimensions));
	currentObject = &objects[0];
}

void MultiObject::draw(GLint iModelViewProjectionLoc, glm::mat4 const& mvp)
{
	currentObject->draw(iModelViewProjectionLoc, mvp);
}

void MultiObject::update(float dt)
{
	currentObject->setPos(getPos());
	currentObject->update(dt);
}

void MultiObject::addObject(GameObject* newObj)
{
	objects.push_back(*newObj);
}

void MultiObject::setActive(int it)
{
	try
	{
		currentObject = &objects.at(it);
	}
	catch (std::out_of_range err)
	{
		std::cerr << err.what() << std::endl;
	}
}



/////////////////////////////////////////////////////////////////////  Menu  //

Menu::Menu()
{
	state = MENU_STATE::MAIN;
	p1Ready = p2Ready = false;

	////////////////////////////  Ready objects
	//characterReady.push_back(MultiObject(""));	// Ready note
	//characterReady.push_back(MultiObject());	// Blue not ready
	//characterReady.push_back(MultiObject());	// Red not ready

	//characterReady[1].addObject();	// Blue ready
	//characterReady[2].addObject();	// Red ready

	//////////////////////////////  Health objects
	//health.push_back(MultiObject());	// Blue Health
	//health.push_back(MultiObject());	// Blue Health
	//health[0].addObject();	// Blue no health
	//health[1].addObject();	// Red no health

	//////////////////////////////  Menu objects

}

void Menu::draw()
{

}

void Menu::update(float dt)
{

}