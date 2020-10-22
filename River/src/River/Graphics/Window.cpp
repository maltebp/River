#include "Window.h"

#include <string>
#include <iostream>

#include "River/Event/MouseEvent/MouseEventController.h"



namespace River {

	// GLFW error callback
	static int glfwErrorCode = 0;
	static std::string glfwErrorMsg;
	static void glfwErrorCallback(int errCode, const char* errStr) {
		glfwErrorCode = errCode;
		glfwErrorMsg = std::string(errStr);
	}

	std::unordered_map<GLFWwindow*, Window*> Window::glfwWindowMap;


	/**
	 * @brief	Converts a glfw mouse button code to a River::MouseButton
	*/
	MouseButton glfwConvertMouseButton(int glfwCode) {
		switch( glfwCode ) {
			case GLFW_MOUSE_BUTTON_LEFT:
				return MouseButtons::LEFT;
			case GLFW_MOUSE_BUTTON_RIGHT:
				return MouseButtons::RIGHT;
			case GLFW_MOUSE_BUTTON_MIDDLE:
				return MouseButtons::MIDDLE;
			case GLFW_MOUSE_BUTTON_4:
				return MouseButtons::EXTRA1;
			case GLFW_MOUSE_BUTTON_5:
				return MouseButtons::EXTRA2;
			case GLFW_MOUSE_BUTTON_6:
				return MouseButtons::EXTRA3;
			case GLFW_MOUSE_BUTTON_7:
				return MouseButtons::EXTRA4;
			case GLFW_MOUSE_BUTTON_8:
				return MouseButtons::EXTRA5;
		}
		// TODO: Use logging system here (shouldn't happen)
		std::cout << "WARNING - Unknown mouse action code: " << glfwCode << std::endl;
		return MouseButtons::UNKNOWN;
	}


	/**
	 * @brief	Converts a glfw mouse button action, to a River::MouseButtonAction
	*/
	MouseButtonAction glfwConvertMouseAction(int glfwCode) {
		if( glfwCode == GLFW_PRESS ) return MouseButtonAction::DOWN;
		if( glfwCode == GLFW_RELEASE ) return MouseButtonAction::UP;
		// TODO: Log some  (shouldn't happen)
		std::cout << "WARNING - Unknown mouse action code: " << glfwCode << std::endl;
		return MouseButtonAction::UNKNOWN;
	}


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

		// TODO: Figure out to handle sampling per texture
		//glfwWindowHint(GLFW_SAMPLES, 16);

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
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS); // TODO: This is redundandt (GL_LESS is default)
		);

		// This enable binary alpha blending
		glAlphaFunc(GL_GREATER, 0);

		// TODO: Move this into a seperate function
		GL( glClearColor(0.15f, 0.8f, 0.15f, 1.0f) );

		previousFpsTime = glfwGetTime();

		double mouseX, mouseY;
		glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
		MouseEventController::initialize(mouseX, mouseY);

		glfwSetCursorPosCallback(glfwWindow, glfwMousePosCallback);
		glfwSetScrollCallback(glfwWindow, glfwMouseScrollCallback);
		glfwSetMouseButtonCallback(glfwWindow, glfwMouseButtonCallback);
	}


	Window::~Window() { }



	void Window::clear() {	

		// Measure speed
		double currentTime = glfwGetTime();
		frameCount++;
		// If a second has passed.
		double timePassed = currentTime - previousFpsTime;
		if( timePassed >= 1.0 ) {
			fps = frameCount / timePassed;

			frameCount = 0;
			previousFpsTime = currentTime;
		}

		glfwSwapBuffers(this->glfwWindow);
		glfwPollEvents();
		GL(glDepthMask(GL_TRUE)); // We must be able to write to the depth buffer in order to clear the bit
		GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}


	void Window::close() {
		glfwSetWindowShouldClose(glfwWindow, GL_TRUE);
	}

	bool Window::shouldClose() {
		return glfwWindowShouldClose(glfwWindow);
	}


	void Window::clearDepth() {
		GL(glClear(GL_DEPTH_BUFFER_BIT));
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



	void Window::glfwMousePosCallback(GLFWwindow* glfwWindow, double mouseX, double mouseY) {
		MouseEventController::registerMouseMovement(mouseX, mouseY);
	}


	void Window::glfwMouseScrollCallback(GLFWwindow* glfwWindow, double xOffset, double yOffset) {
		MouseEventController::registerMouseScroll(yOffset);
		// yOffset is the classic scroll direction, while xOffset is also used for controllers,
		// which are not supported in this framework yet
	}

	void Window::glfwMouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mode) {
		MouseEventController::registerMouseButtonAction(
			glfwConvertMouseButton(button),
			glfwConvertMouseAction(action)
		);
	}


	std::vector<KeyEvent> Window::getKeyEvents(){
		return keyEventController.getEvents();
	}	



	double Window::getFps() {
		return fps;
	}


}
