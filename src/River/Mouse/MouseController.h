#pragma once

#include <unordered_set>

#include "Mouse.h"
#include "River/Primitives/MouseButton.h"


namespace River {

	class MouseController {
		friend Mouse;
	public:

		static void initialize(double mouseX, double mouseY);

		static void registerMouseMovement(double newX, double newY);

		static void registerMouseScroll(double amount);

		static void registerButtonDown(MouseButton button);

		static void registerButtonUp(MouseButton button);

		static void invokeEvents();

	private:

		// Holds information about a key's events in this event cycle
		struct ButtonEventState {
			bool down = false;
			bool up = false;
			bool pressed = false;
		};

	private:

		static inline double newMouseX = 0;
		static inline double newMouseY = 0;

		static inline double currentMouseX = 0;
		static inline double currentMouseY = 0;

		static inline bool mouseMoved = 0;

		static inline double mouseScrollAmount = 0;

		/**
		 * @brief	Maps a MouseButton code to its event state
		 * @details	Once an event is added to this, it will remain there for the rest of the program
		 *			We're using the button code instead of the button object, as its comparison operator has been overloaded,
		 *			and thus its hashing functionality is out of order (for now)
		*/
		static inline std::unordered_map<uint32_t, ButtonEventState>  buttonEventStates;

		// List of mouse buttons that are pressed this event cycle
		static inline std::unordered_set<uint32_t> pressedEvents;

		static inline ListenerInvoker moveInvoker = (Mouse::moveListeners);

		static inline ListenerInvoker scrollInvoker = (Mouse::scrollListeners);

		static inline ListenerInvoker buttonDownInvoker = (Mouse::buttonDownListeners);

		static inline ListenerInvoker buttonUpInvoker = (Mouse::buttonUpListeners);

	};

}


