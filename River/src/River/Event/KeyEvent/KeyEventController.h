#pragma once


#include <unordered_map>

#include "Key.h"
#include "KeyEvent.H"

namespace River {



	/**
	 * @brief	Handles registration and querying of events
	 *
	 *
	 * @details		Events are handled "per cycle", where each cycle is seperated by a call to "getEvents". In each cycle,
	 *				a maxiumum of one of each events can occur for each key regardless of the events registered.
	*/
	class KeyEventController {


		// Holds information about a key's events in this event cycle
		struct KeyEventState{
			bool down = false;
			bool up = false;
			bool pressed = false;
		};

		std::unordered_map<Key, KeyEventState>  keyEventStates;

	public:


		/**
		 * @brief	Register that some action was performed on a particular key. The event fired from this registration
		 *			will depend on the key's state at registration.

		 * @param	key	The key on which the action occured
		 * @param	action	The action which occured
		*/
		void registerEvent(Key key, KeyEvent::Action action);


		/**
		 * @brief	Returns a list of events that happened since last time the events were queried (since last event cycle)
		 *
		*/
		std::vector<KeyEvent> getEvents();
	};

}


