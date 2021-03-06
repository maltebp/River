#include "River/pch.h"

#include "MouseController.h"

#include "Mouse.h"
#include "MouseMoveEvent.h"
#include "MouseScrollEvent.h"


namespace River {


	namespace {
		std::vector<MouseButton> buttonUpEvents;
		std::vector<MouseButton> buttonDownEvents;
	}


	void MouseController::initialize(double mouseX, double mouseY) {
		currentMouseX = mouseX;
		currentMouseY = mouseY;
	}


	void MouseController::registerMouseMovement(double mouseX, double mouseY) {
		newMouseX = mouseX;
		newMouseY = mouseY;
		mouseMoved = true;
	}


	void MouseController::registerMouseScroll(double amount) {
		 mouseScrollAmount += amount;
	}


	void MouseController::registerButtonDown(MouseButton button) {
		 ButtonEventState& eventState = buttonEventStates[button.code];

		 if( eventState.pressed ) return;

		 eventState.down = true;
		 eventState.pressed = true;
	}


	void MouseController::registerButtonUp(MouseButton button) {
		ButtonEventState& eventState = buttonEventStates[button.code];

		if( !eventState.pressed ) return;

		eventState.up = true;
		eventState.pressed = false;
	}


	void MouseController::invokeEvents() {
		pressedEvents.clear();
		buttonUpEvents.clear();
		buttonDownEvents.clear();

		// Create mouse button events
		for( auto& pair : buttonEventStates ) {
			MouseButton button = pair.first;
			ButtonEventState& buttonEventState = pair.second;

			if( buttonEventState.down ) {
				buttonDownEvents.push_back(button);
			}

			if( buttonEventState.up ) {
				buttonUpEvents.push_back(button);
			}

			// Update button press
			if( buttonEventState.pressed && !buttonEventState.down ) {
				pressedEvents.insert(button.code);
			}

			// Reset event values (pressed is updated when registering event) 
			buttonEventState.down = false;
			buttonEventState.up = false;
		}

		// Invoke button down events
		for( auto& button : buttonDownEvents ) {
			MouseButtonEvent buttonEvent(button);

			for( auto& iterator : buttonDownInvoker.listeners ) {
				if( buttonEvent.isConsumed() ) break;
				auto listener = iterator.second;
				listener(buttonEvent);
			}
		}

		// Invoke Button Up events
		for( auto& button : buttonUpEvents ) {
			MouseButtonEvent buttonEvent(button);

			for( auto& iterator : buttonUpInvoker.listeners ) {
				if( buttonEvent.isConsumed() ) break;
				auto listener = iterator.second;
				listener(buttonEvent);
			}
		}

		// Invoke Move event
		if( mouseMoved ) {
			mouseMoved = false;

			double mouseMoveX = newMouseX - currentMouseX;
			double mouseMoveY = newMouseY - currentMouseY;

			currentMouseX = newMouseX;
			currentMouseY = newMouseY;

			MouseMoveEvent event(newMouseX, newMouseY, mouseMoveX, mouseMoveY);

			for( auto& iterator : moveInvoker.listeners ) {
				if( event.isConsumed() ) break;
				auto listener = iterator.second;
				listener(event);
			}
		}

		// Scroll event
		if( mouseScrollAmount != 0 ) {
			MouseScrollEvent event(mouseScrollAmount);
			
			for( auto& iterator : scrollInvoker.listeners ) {
				if( event.isConsumed() ) break;
				auto listener = iterator.second;
				listener(event);
			}

			mouseScrollAmount = 0;
		}
	}


}