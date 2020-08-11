#pragma once

#include <vector>

#include "Window.h"
#include "TextureBinder.h"

namespace River {
	class Renderer {


	protected:
		Window *window;
		Renderer(Window* window);

	public:

		
 		void flush();
		virtual void onFlush(){ };

	};
}


