#pragma once

#include <algorithm>


namespace River {

	struct Color {

		Color() { }

		Color(float c) 
			: r(c), b(c), g(c)
		{ }

		Color(float r, float g, float b, float a)
			: r(r),  b(b), g(g), a(a)
		{ }
		
		Color(float r, float g, float b )
			: r(r),  b(b), g(g) 
		{ }	

		/**
		 * @return	Whether alpha value is between 0 and 1 excluding both
		*/
		bool isPartiallyTransparent() const {
			return a > 0.0f && a < 1.0f;
		}

		/**
		 * @brief	Clamps all values to 0 and 1 
		*/
		void clamp() {
			r = std::clamp(r, 0.0f, 1.0f);
			g = std::clamp(g, 0.0f, 1.0f);
			b = std::clamp(b, 0.0f, 1.0f);
			a = std::clamp(a, 0.0f, 1.0f);
		}


		bool operator ==(const Color& other) const {
			return	
				r == other.r &&
				g == other.g &&
				b == other.b &&
				a == other.a;
		}


		bool operator !=(const Color& other) const {
			return
				r != other.r ||
				g != other.g ||
				b != other.b ||
				a != other.a;
		}


	public:

		float r = 1.0f;
		
		float g = 1.0f;
		
		float b = 1.0f;
		
		float a = 1.0f;

	};
	

	/**
	 * @brief	Const collection of pre-defined colors
	*/
	namespace Colors {

		// Basic colors
		const Color BLACK		(0.0f, 0.0f, 0.0f);
		const Color WHITE		(1.0f, 1.0f, 1.0f);
		const Color RED			(1.0f, 0.0f, 0.0f);
		const Color GREEN		(0.0f, 1.0f, 0.0f);
		const Color BLUE		(0.0f, 0.0f, 1.0f);
		const Color YELLOW		(1.0f, 1.0f, 0.0f);
		const Color MAGENTA		(1.0f, 0.0f, 1.0f);
		const Color CYAN		(0.0f, 1.0f, 1.0f);

		// Special colors
		const Color LAWN_GREEN	(0.486f, 0.988f, 0.0f);
	}
}