#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameEngine.h"

// Abstract class for the states
class GameState
{
public:
	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;
	
	// handle input from user
	virtual void HandleEvents(GameEngine* game) = 0;

	// update enemy AI and physics
	virtual void Update(GameEngine* game, float deltaTime) = 0;

	// draw the information to the screen
	virtual void Draw(GameEngine* game) = 0;

	void ChangeState(GameEngine* game, GameState* state) {
		game->ChangeState(state);
	}

protected:
	GameState() { }
};


#endif