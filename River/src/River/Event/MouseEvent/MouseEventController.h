#pragma once

#include <vector>

#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"
#include "MouseScrollEvent.h"


namespace River {
	
	class MouseEventController {
	public:



		/**
		 * @brief	Initializes the controller, currently by setting the mouse initial position
		*/
		static void initialize(double mouseX, double mouseY);
		
		static void registerMouseMovement(double newX, double newY);

		static bool mouseMovementOccured();

		static MouseMoveEvent getMouseMoveEvent();
		

	private:
		static bool initialized;

		static std::vector<MouseButtonEvent> buttonEvents;
		static MouseMoveEvent moveEvent;
		static MouseScrollEvent scrollEvent;


		// Whether or not any mouse movement has happened
		static bool mouseMovement;
		static double newMouseX, newMouseY;
		static double currentMouseX, currentMouseY;
		
	};
}

