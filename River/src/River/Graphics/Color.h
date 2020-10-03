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
	
		// TODO: Create proper operator overloads
	};
	

	namespace Colors {
		const Color BLACK		(0.0, 0.0, 0.0);
		const Color WHITE		(1.0, 1.0, 1.0);
		const Color RED			(1.0, 0.0, 0.0);
	}
}