#pragma once

namespace River {

	class MouseScrollEvent : public Event {
	public:

		MouseScrollEvent(double amount)
			: amount(amount)
		{}

	public:

		/**
		 * @brief	The amount scrolled. Positive is typically upwards (scrolling mousewheel away), and negative is downwards (scrolling mousewheel towards you)
		*/
		const double amount;

	};

}


