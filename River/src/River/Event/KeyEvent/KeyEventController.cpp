#include "KeyEventController.h"

#include <iostream>
#include <vector>

#include "River/Graphics/GL.h"

namespace River {

	void KeyEventController::registerEvent(Key key, KeyEvent::Action action){

		// Fetch they KeyEventState for the key, and update cache duration
		auto pair = keyEventStates.find(key);
		if( pair == keyEventStates.end() )
			pair = keyEventStates.emplace(key, KeyEventState{}).first;
		auto& keyEventState = pair->second;
		keyEventState.cacheDuration = CACHE_DURATION;

		if( action == KeyEvent::Action::DOWN ){
			if( keyEventState.pressed ) return;
			keyEventState.down = true;
			keyEventState.pressed = true;
		}

		if( action == KeyEvent::Action::UP ){
			if( !keyEventState.pressed ) return;
			keyEventState.up = true;				
		}
	}


	std::vector<KeyEvent> KeyEventController::getEvents(){
		std::vector<KeyEvent> events;
		std::vector<Key> statesToRemove;
		
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

			// Update / check cache duration
			if( !keyEventState.pressed ){
				keyEventState.cacheDuration--;
				if( keyEventState.cacheDuration <= 0 ){
					statesToRemove.push_back(key);
				}
			}
		}

		// Clear old states
		for( Key key : statesToRemove )
			keyEventStates.erase(key);

		return events;
	}

}