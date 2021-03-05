#pragma once

#include "River/Utility/Event.h"


namespace River {

	struct MouseMoveEvent : Event {	
	public:

		MouseMoveEvent(double positionX, double positionY, double movementX, double movementY)
			: positionX(positionX), positionY(positionY), movementX(movementX), movementY(movementY)
		{}


	public:

		// New position
		const double positionX;
		const double positionY;
		
		// How much it has moved
		const double movementX;
		const double movementY;

	};

}


