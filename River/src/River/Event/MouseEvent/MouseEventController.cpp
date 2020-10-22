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
	// Mouse Button Event

	void MouseEventController::registerMouseButtonAction(MouseButton button, MouseButtonAction action) {

		auto& eventState = buttonEventStates[button.code];

		// Set key states
		if( action == MouseButtonAction::DOWN ) {
			if( eventState.pressed ) return;
			eventState.down = true;
			eventState.pressed = true;
		}

		if( action == MouseButtonAction::UP ) {
			if( !eventState.pressed ) return;
			eventState.up = true;
		}
	}

	std::vector<MouseButtonEvent> MouseEventController::getMouseButtonEvents() {
		std::vector<MouseButtonEvent> events;

		// Create events from event states
		for( auto& pair : buttonEventStates ) {
			auto& buttonCode = pair.first;
			auto& eventState = pair.second;

			if( eventState.down ) events.emplace_back(MouseButton(buttonCode), MouseButtonAction::DOWN);
			if( eventState.pressed ) events.emplace_back(MouseButton(buttonCode), MouseButtonAction::PRESSED);
			if( eventState.up ) events.emplace_back(MouseButton(buttonCode), MouseButtonAction::UP);

			// Reset event values
			if( eventState.up ) eventState.pressed = false;
			eventState.down = false;
			eventState.up = false;
		}

		return events;
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

	std::unordered_map<uint32_t, MouseEventController::ButtonEventState> MouseEventController::buttonEventStates;
}