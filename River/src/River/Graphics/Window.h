#pragma once

#include <unordered_map>
#include <vector>

#include "River/Event/KeyEvent/KeyEventController.h"
#include "River/Error.h"
#include "River/Graphics/GL.h"


namespace River {

	class Window {
		static std::unordered_map<GLFWwindow *, Window *> glfwWindowMap;
		static void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);


	private:

        GLFWwindow* glfwWindow;
        int width;
        int height;
        std::string title = "River Window";

		/* The number of texture slots (or units) accessible from the fragment shader */
		int numTextureSlots;


		KeyEventController keyEventController;



	public:

		Window(std::string title, unsigned int width, unsigned int height);
		~Window();

		void clear();
		void clearDepth();
		bool shouldClose();
		unsigned int getNumTextureSlots();
		std::vector<KeyEvent> getKeyEvents();
	};

}


