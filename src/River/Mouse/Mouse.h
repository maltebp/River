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

		/**
		 * @return Returns mouse x coordinate in pixels on the screen, where
		 *			(0,0) is center of screen, and positive x-axis is to the
		 *			right, and positive y-axis is upwards
		*/
		static double getPositionX();

		/**
		 * @return	Returns mouse y coordinate in pixels on the screen, where
		 *			(0,0) is center of screen, and positive x-axis is to the
		 *			right, and positive y-axis is upwards
		*/
		static double getPositionY();

	};

}


