#include "KeyboardController.h"

#include "Keyboard.h"

namespace River {


	namespace {
		std::vector<Key> keyUpEvents;
		std::vector<Key> keyDownEvents;
		std::unordered_set<char32_t> characterEvents;
	}


	void KeyboardController::registerKeyDown(Key key) {

		// Fetch they KeyEventState for the key
		KeyEventState& keyEventState = keyEventStates[key];

		if( keyEventState.pressed ) return;

		keyEventState.down = true;
		keyEventState.pressed = true;
	}


	void KeyboardController::registerKeyUp(Key key) {

		// Fetch they KeyEventState for the key
		KeyEventState& keyEventState = keyEventStates[key];

		if( !keyEventState.pressed ) return;

		keyEventState.up = true;
		keyEventState.pressed = false;

		std::cout << "Key pressed = false" << std::endl;
	}


	void KeyboardController::registerCharacterEvent(char32_t character) {
		characterEvents.insert(character);
	}


	void KeyboardController::invokeEvents() {

		pressedEvents.clear();
		keyUpEvents.clear();
		keyDownEvents.clear();

		// Create events from event states
		for( auto& pair : keyEventStates ) {
			Key key = pair.first;
			KeyEventState& keyEventState = pair.second;

			if( keyEventState.down ) {
				keyDownEvents.push_back(key);
			}

			if( keyEventState.up ) {
				keyUpEvents.push_back(key);
			}

			// Update key press
			if( keyEventState.pressed && !keyEventState.down ) {
				pressedEvents.insert(key);
			}

			// Reset event values (pressed is updated when registering event) 
			keyEventState.down = false;
			keyEventState.up = false;
		}

		// Invoke key down events
		for( auto& key : keyDownEvents ) {
			KeyEvent keyDownEvent(key);

			for( auto& iterator : Keyboard::keyDownInvoker.listeners ) {
				if( keyDownEvent.isConsumed() ) break;
				auto listener = iterator.second;
				listener(keyDownEvent);
			}
		}

		// Invoke key up events
		for( auto& key : keyUpEvents ) {
			KeyEvent keyUpEvent(key);

			for( auto& iterator : Keyboard::keyUpInvoker.listeners ) {
				if( keyUpEvent.isConsumed() ) break;
				auto listener = iterator.second;
				listener(keyUpEvent);
			}
		}

		// Invoke character input events 
		for( char32_t character : characterEvents ) {
			CharacterEvent event(character);
			
			for( auto& iterator : Keyboard::characterInputInvoker.listeners ) {
				if( event.isConsumed() ) break;
				auto listener = iterator.second;
				listener(event);
			}
		}

		characterEvents.clear();
	}


}