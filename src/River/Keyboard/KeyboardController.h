#pragma once

#include <unordered_set>

#include "River/Game.h"
#include "Key.h"


namespace River {

	class Keyboard;

	class KeyboardController {
		friend Game;
		friend Keyboard;
	public:

		// Holds information about a key's events in this event cycle
		struct KeyEventState {
			bool down = false;
			bool up = false;
			bool pressed = false;
		};

		static void registerKeyUp(Key key);

		static void registerKeyDown(Key key);

		static void registerCharacterEvent(char32_t character);

		static void invokeEvents();


	private:

		static inline std::unordered_map<Key, KeyEventState>  keyEventStates;

		// Keys that are pressed this event cycle
		static inline std::unordered_set<Key> pressedEvents;

	};

}

