#ifndef BATTLE_STATE_H
#define BATTLE_STATE_H

#include "GameState.h"
#include "Fizzle\DataTypes\FizzleDataTypes.h"
#include <map>
#include <vector>

// Forward Declaration
namespace InputMapping
{
	struct MappedInput;
}

namespace ExploreState
{
	class ExploreState : public GameState
	{
	public:
		// Initialize the state
		void Init();

		// Cleanup the state before exiting
		void Cleanup();

		// Pause or resume the state before/after changing
		void Pause();
		void Resume();

		// Operations done by the state, can call to the game engine
		void HandleEvents(GameEngine* game);
		void Update(GameEngine* game);
		void Draw(GameEngine* game);

		// Return an instance of the object
		static ExploreState* Instance() {
			return &m_ExploreState;
		}

		// The callback used in the state
		friend void mainCallback(InputMapping::MappedInput& inputs);

	protected:
		// Create an empty object
		ExploreState() { }

	private:
		static ExploreState m_ExploreState;
		std::vector<FzlKey> validKeys;
		std::map<FzlKey, bool> previouslyPressed;
	};
}

#endif