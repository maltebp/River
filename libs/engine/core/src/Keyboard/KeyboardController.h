#pragma once

#include "River/Game.h"
#include "River/Primitives/KeyboardKey.h"


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

		static void registerKeyUp(KeyboardKey key);

		static void registerKeyDown(KeyboardKey key);

		static void registerCharacterEvent(char32_t character);

		static void invokeEvents();


	private:

		static inline std::unordered_map<KeyboardKey, KeyEventState>  keyEventStates;

		// Keys that are pressed this event cycle
		static inline std::unordered_set<KeyboardKey> pressedEvents;

	};

}

