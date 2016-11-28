#include "gl\glew.h"
#include "gl\freeglut.h"
#include "glm\glm.hpp"

#include "GameEngine.h"
#include "GameState.h"


int GameEngine::Init()
{


	return 0;
}

void GameEngine::Cleanup()
{
	// Clean up all states
	while (!states.empty())
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	
}

void GameEngine::ChangeState(GameState* state)
{
	// Clean up the current state
	if (!states.empty()) 
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void GameEngine::PushState(GameState* state)
{
	// pause current state
	if (!states.empty())
	{
		states.back()->Pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void GameEngine::PopState()
{
	// cleanup the current state
	if (!states.empty())
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if (!states.empty()) 
	{
		states.back()->Resume();
	}
}

void GameEngine::HandleEvents()
{
	// let the state handle events
	states.back()->HandleEvents(this);
}

void GameEngine::Update(float dt)
{
	// let the state update the game
	states.back()->Update(this, dt);
}

void GameEngine::Draw()
{
	// let the state draw the screen
	states.back()->Draw(this);
}