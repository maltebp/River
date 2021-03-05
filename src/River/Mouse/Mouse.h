#pragma once

#include "River/Utility/Listener.h"
#include "MouseMoveEvent.h"
#include "MouseScrollEvent.h"
#include "MouseButtonEvent.h"

namespace River {

	class MouseController;

	class Mouse {
		friend MouseController;
	public:

		static inline ListenerMap<MouseMoveEvent> moveListeners;

		static inline ListenerMap<MouseScrollEvent> scrollListeners;

		static inline ListenerMap<MouseButtonEvent> buttonDownListeners;

		static inline ListenerMap<MouseButtonEvent> buttonUpListeners;

		static bool isButtonPressed(MouseButton button, bool consume = false);

		static double getPositionX();

		static double getPositionY();

	};

}


