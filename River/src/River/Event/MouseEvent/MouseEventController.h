#pragma once

#include <vector>

#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"
#include "MouseScrollEvent.h"


namespace River {
	
	class MouseEventController {
	public:


		/**
		 * @brief	Initializes the controller by setting the mouse initial position
		*/
		static void initialize(double mouseX, double mouseY);
		
		static void registerMouseMovement(double newX, double newY);

		static bool hasMovementOccured();

		static MouseMoveEvent getMouseMoveEvent();


		static void registerMouseScroll(double amount);

		/**
		 * @returns	Whether any scrolling has occured (registered through the registerMouseScroll(..) method).
		 *			If false, you can safely call getMouseScrollEvent(..) without getting an exception		
		*/
		static bool hasScrollingOccured();

		/**
		 * @returns A MouseScrollEvent with the amount scrolled since last call to this method
		 * @throws	River::NullException	If no mouse scrolling has happened since last call to this method. Check result of hasScrollingOccured() to avoid this
		*/
		static MouseScrollEvent getMouseScrollEvent();
		

	private:
		static bool initialized;

		static std::vector<MouseButtonEvent> buttonEvents;
		static MouseMoveEvent moveEvent;
		static MouseScrollEvent scrollEvent;

		static double mouseScrollAmount;

		// Whether or not any mouse movement has happened
		static bool mouseMovement;
		static double newMouseX, newMouseY;
		static double currentMouseX, currentMouseY;
		
	};
}

