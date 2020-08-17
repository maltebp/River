#pragma once

#include <iostream>

#include "River/Error.h"
#include "River/Graphics/GL.h"

namespace River {

	class Window {


	private:
        GLFWwindow* glfwWindow;
        int width;
        int height;
        std::string title = "River Window";

		/* The number of texture slots (or units) accessible from the fragment shader */
		int numTextureSlots;

	public:

		Window(std::string title, unsigned int width, unsigned int height);
		~Window();

		void clear();
		void clearDepth();
		bool shouldClose();
		unsigned int getNumTextureSlots();
	};

}


