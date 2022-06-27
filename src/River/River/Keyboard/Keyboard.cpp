#include "River/pch.h"

#include "Keyboard.h"

#include "KeyboardController.h"


namespace River {


	bool Keyboard::isKeyPressed(KeyboardKey key, bool consume) {
		auto iterator = KeyboardController::pressedEvents.find(key);
		if( iterator == KeyboardController::pressedEvents.end() ) {
			return false;
		}

		if( consume ) {
			KeyboardController::pressedEvents.erase(key);
		}

		return true;
	}

}