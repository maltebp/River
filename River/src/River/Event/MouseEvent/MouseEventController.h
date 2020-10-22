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



		static void registerMouseButtonAction(MouseButton btn, MouseButtonAction action);

		static std::vector<MouseButtonEvent> getMouseButtonEvents();
		

	public:
		// Holds information about a key's events in this event cycle
		struct ButtonEventState {
			bool down = false;
			bool up = false;
			bool pressed = false;
		};

	private:
		static bool initialized;

		static double mouseScrollAmount;

		// Whether or not any mouse movement has happened
		static bool mouseMovement;
		static double newMouseX, newMouseY;
		static double currentMouseX, currentMouseY;

		/**
		 * @brief	Maps a MouseButton code to its event state
		 * @details	Once an event is added to this, it will remain their for the rest of the program
		 *			We're using the button code instead of the button object, as its comparison operator has been overloaded,
		 *			and thus its hashing functionality is out of order (for now)
		*/
		static std::unordered_map<uint32_t, ButtonEventState>  buttonEventStates;
	};
}

