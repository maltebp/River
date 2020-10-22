#include "KeyEventController.h"

#include <iostream>
#include <vector>

#include "River/Graphics/GL.h"

namespace River {

	void KeyEventController::registerEvent(Key key, KeyEvent::Action action){

		// Fetch they KeyEventState for the key, and update cache duration
		auto& keyEventState = keyEventStates[key];

		// Set key states
		if( action == KeyEvent::Action::DOWN ){
			if( keyEventState.pressed ) return;
			keyEventState.down = true;
			keyEventState.pressed = true;
		}

		if( action == KeyEvent::Action::UP ){
			if( !keyEventState.pressed ) return;
			keyEventState.up = true;				
		}

		// It's a bit weird, that the PRESSED action is not handled
		// as it can be passed to the function?
	}



	std::vector<KeyEvent> KeyEventController::getEvents(){
		std::vector<KeyEvent> events;
	
		// Create events from event states
		for( auto &pair : keyEventStates ){
			Key key = pair.first;
			KeyEventState &keyEventState = pair.second;

			if( keyEventState.down ) events.emplace_back(key, KeyEvent::Action::DOWN);
			if( keyEventState.pressed ) events.emplace_back(key, KeyEvent::Action::PRESSED);
			if( keyEventState.up ) events.emplace_back(key, KeyEvent::Action::UP);

			// Reset event values
			if( keyEventState.up ) keyEventState.pressed = false;
			keyEventState.down = false;
			keyEventState.up = false;
		}

		return events;
	}

}