#pragma once

#include "River/Utility/Event.h"
#include "River/Primitives/MouseButton.h"

namespace River {

	/**
	 * @brief	The type of event action
	*/
	enum class MouseButtonAction {
		UNKNOWN,

		/**
		 * @brief	Signals that a mouse button was up, but got pressed down
		*/
		DOWN,

		/**
		 * @brief	Signals that a mouse button was pressed (held down) this event cycle. A pressed events will occur each cycle,
		 *			until the button is released again, and an up event will be fired
		*/
		PRESSED,

		/**
		 * @brief	Signals that a was down and was released "up"
		*/
		UP
	};

	struct MouseButtonEvent : public Event {
	public:

		MouseButtonEvent(const MouseButton button)
			: button(button)
		{}


	public:

		const MouseButton button;

	};

}


