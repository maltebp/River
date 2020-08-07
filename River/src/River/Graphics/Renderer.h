#pragma once

#include "Window.h"

namespace River {
	class Renderer {

	protected:
		Window *window;

	public:

		Renderer(Window *window);

			

 		virtual void flush() = 0;

	};
}


