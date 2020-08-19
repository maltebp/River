#include "Window.h"

#include <string>

#include <iostream>



namespace River {

	// GLFW error callback
	static int glfwErrorCode = 0;
	static std::string glfwErrorMsg;
	static void glfwErrorCallback(int errCode, const char* errStr) {
		glfwErrorCode = errCode;
		glfwErrorMsg = std::string(errStr);
	}

	std::unordered_map<GLFWwindow*, Window*> Window::glfwWindowMap;



	Window::Window(std::string title, unsigned int width, unsigned int height) {
		this->title = title;

		// TODO: Make the user able to adjust this
		this->width = width;
		this->height = height;

		// Initialize glfw
		if (!glfwInit()) {   
			throw River::Exception("GLFW initialization error '" + glfwErrorMsg + "'" + " (code: " + std::to_string(glfwErrorCode) + ")");
		}

		glfwSetErrorCallback(glfwErrorCallback);
		glfwWindowHint(GLFW_SAMPLES, 16);
		glfwWindow = glfwCreateWindow(width, height, this->title.c_str(), NULL, NULL);
		if (!glfwWindow) {
			glfwTerminate();
			throw River::Exception("GLFW initialization error '" + glfwErrorMsg + "'" + " (code: " + std::to_string(glfwErrorCode) + ")");
		}

		glfwSetKeyCallback(glfwWindow, glfwKeyCallback);

		glfwWindowMap[glfwWindow] = this;

		// Set window to current
		glfwMakeContextCurrent(glfwWindow);

		// Get number of texture slots
		GL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &numTextureSlots));
	
		GL(
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		);
	}


	Window::~Window() { }



	void Window::clear() {
		glfwSwapBuffers(this->glfwWindow);
		glfwPollEvents();
		GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}


	bool Window::shouldClose() {
		return glfwWindowShouldClose(glfwWindow);
	}


	void Window::clearDepth() {
		// TODO: Implement depth buffer stuff here
	}

	unsigned int Window::getNumTextureSlots() {
		if (numTextureSlots == 0)
			throw River::Exception("Number of texture slots is 0");
		return numTextureSlots;
	}

	void Window::glfwKeyCallback(GLFWwindow *glfwWindow, int glfwKey, int scancode, int glfwAction, int mods){
		Window *window = glfwWindowMap[glfwWindow];

		Key key = (Key) glfwKey;

		KeyEvent::Action action;
		switch( glfwAction ){
		case GLFW_PRESS:
			action = KeyEvent::Action::DOWN;
			break;
		case GLFW_RELEASE:
			action = KeyEvent::Action::UP;
			break;
		case GLFW_REPEAT: return; // We don't use this
		default:
			// TODO: Implement correct logging
			std::cout << "WARNING: GLFW action '" << glfwAction << "' is not handled!" << std::endl;
			return;
		}

		window->keyEventController.registerEvent(key, action);
	}


	std::vector<KeyEvent> Window::getKeyEvents(){
		return keyEventController.getEvents();
	}




}
