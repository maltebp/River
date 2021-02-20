#pragma once

namespace River {

	/**
	 * @brief	Represents a screen resolution with a width and height
	*/
	struct Resolution {
		
		Resolution();

		Resolution(unsigned int width, unsigned int height);

		/**
		 * @return	The width/height ratio of this resolution,
		 *			or 0 if either width or height is 0
		*/
		double getAspectRatio();

		/**
		 * @return	True if both the width and height
		 *			of this and other are the same
		*/
		bool operator ==(const Resolution& other);
		bool operator !=(const Resolution& other);

		unsigned int width = 0;
		unsigned int height = 0;
	};

}
