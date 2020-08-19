#pragma once


#include <unordered_map>

#include "Key.h"
#include "KeyEvent.H"

namespace River {

	class KeyEventController {

		// Number of event-cycles to persist a non-used KeyEventState
		static const unsigned int CACHE_DURATION = 500;

		// Holds information about a key's events in this event cycle
		struct KeyEventState{
			bool down = false;
			bool up = false;
			bool pressed = false;
			unsigned int cacheDuration = 0;
		};

		std::unordered_map<Key, KeyEventState>  keyEventStates;

	public:
		void registerEvent(Key key, KeyEvent::Action action);
		std::vector<KeyEvent> getEvents();
	};

}


