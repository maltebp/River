#include "River/pch.h"

#include "Mouse.h"

#include "MouseController.h"

namespace River {


	bool Mouse::isButtonPressed(MouseButton button, bool consume) {
		auto iterator = MouseController::pressedEvents.find(button.code);
		if( iterator == MouseController::pressedEvents.end() ) {
			return false;
		}

		if( consume ) {
			MouseController::pressedEvents.erase(button.code);
		}

		return true;
	}


	double Mouse::getPositionX() {
		return MouseController::currentMouseX;
	}


	double Mouse::getPositionY() {
		return MouseController::currentMouseY;
	}

}