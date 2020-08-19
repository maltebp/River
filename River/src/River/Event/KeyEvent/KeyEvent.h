#pragma once

#include "Key.h"
#include "../Event.h"

namespace River {

	class KeyEvent : public Event {

	public: enum class Action {
		UNKNOWN,
		DOWN,
		PRESSED,
		UP
	};

	public:
		const Key key = Key::A;
		const Action action;

	public:
		KeyEvent(const Key key, const Action action);
	};

}


