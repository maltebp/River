#pragma once

namespace River {

	struct Color {

		float r = 1.0;
		float g = 1.0;
		float b = 1.0;
		float a = 1.0;

	
		Color() {}
		Color(float r, float g, float b, float a) : r(r),  b(b), g(g), a(a) {}
		Color(float r, float g, float b ) : r(r),  b(b), g(g) {}	


		bool isPartiallyTransparent() const {
			return a > 0.0 && a < 1.0;
		}

		// TODO: Create proper operator overloads
	};
	

	namespace Colors {

		// Basic colors
		const Color BLACK		(0.0, 0.0, 0.0);
		const Color WHITE		(1.0, 1.0, 1.0);
		const Color RED			(1.0, 0.0, 0.0);
		const Color GREEN		(0.0, 1.0, 0.0);
		const Color BLUE		(0.0, 0.0, 1.0);
		const Color YELLOW		(1.0, 1.0, 0.0);
		const Color MAGENTA		(1.0, 0.0, 1.0);
		const Color CYAN		(0.0, 1.0, 1.0);

		// Special colors
		const Color LAWN_GREEN	(0.486, 0.988, 0);
	}
}