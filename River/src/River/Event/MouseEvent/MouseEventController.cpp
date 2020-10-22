#include "MouseEventController.h"

#include <iostream>
#include <vector>

#include "River/Error.h"


namespace River {


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Movement event

	void MouseEventController::initialize(double mouseX, double mouseY) {
		if( initialized )
			throw new AlreadyInitializedException("MouseEventController");
		initialized = true;
	}


	void MouseEventController::registerMouseMovement(double mouseX, double mouseY) {
		if( !initialized )
			throw new NotInitializedException("MouseEventController");

		newMouseX = mouseX;
		newMouseY = mouseY;
		mouseMovement = true;
	}


	bool MouseEventController::hasMovementOccured() {
		return mouseMovement;
	}


	MouseMoveEvent MouseEventController::getMouseMoveEvent() {
		if( !mouseMovement )
			throw new NullException("No mouse movement has been registered");

		mouseMovement = false;

		double mouseMoveX = newMouseX - currentMouseX;
		double mouseMoveY = newMouseY - currentMouseY;

		currentMouseX = newMouseX;
		currentMouseY = newMouseY;

		return { newMouseX, newMouseY, mouseMoveX, mouseMoveY };
	}



	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Scroll event		
	
	void MouseEventController::registerMouseScroll(double amount) {
		if( !initialized )
			throw new NotInitializedException("MouseEventController");
		mouseScrollAmount += amount;
	}


	bool MouseEventController::hasScrollingOccured() {
		return mouseScrollAmount != 0;
	}


	MouseScrollEvent MouseEventController::getMouseScrollEvent() {
		if( mouseScrollAmount == 0 )
			throw new NullException("No mouse scrolling has been registered");

		double scrollAmount = mouseScrollAmount;
		mouseScrollAmount = 0;
		return scrollAmount;
	}



	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Static variables
	bool MouseEventController::initialized = false;
	bool MouseEventController::mouseMovement = false;

	double MouseEventController::mouseScrollAmount = 0;

	double MouseEventController::newMouseX = 0;
	double MouseEventController::newMouseY = 0;
	double MouseEventController::currentMouseX = 0;
	double MouseEventController::currentMouseY = 0;
}