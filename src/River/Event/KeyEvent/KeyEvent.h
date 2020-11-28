#pragma once

#include "Key.h"
#include "../Event.h"

namespace River {

	class KeyEvent : public Event {
	public:

		/**
		 * @brief	The type of event action
		*/
		enum class Action {
			UNKNOWN,

			/**
			 * @brief	Signals that a key was up, but got pressed down 
			*/
			DOWN,

			/**
			 * @brief	Signals that a key was pressed (held down) this event cycle. A pressed events will occur each cycle,
		     *			until the key is released again, and an up event will be fired.
			*/
			PRESSED,

			/**
			 * @brief	Signals that a key was down, and has now been releaseed
			*/
			UP
		};

	public:
		const Key key = Key::A;
		const Action action;

	public:
		KeyEvent(const Key key, const Action action);
	};

}


