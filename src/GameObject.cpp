#include "GameObject.h"

GameObject::GameObject()
{
	
}

GameObject::GameObject(char const* filePath)
{
	loadObject(filePath);
}

void GameObject::loadObject(char const* filePath)
{
	obj = Loader(filePath);
}

void GameObject::update(float deltaTime)
{

}