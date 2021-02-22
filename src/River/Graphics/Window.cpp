#include "Window.h"

#include <string>
#include <iostream>

#include "River/Event/MouseEvent/MouseEventController.h"


namespace River {


	class Window::NativeWindow {
	public:
		
		// Callback for GLFW window size change function
		static void windowSizeCallback(GLFWwindow* window, int width, int height) {
			Resolution newResolution((unsigned int)width, (unsigned int)height);

			if( newResolution.width < 0 || newResolution.height < 0 ) {
				// Just a safety precaution (not sure if it's ever possible)
				throw new Exception("GLFW window size callback passed negative width or height");
			}			

			// Window size is set to 0x0 when it gets minimized (both fullscreen and windowed mode)
			if( newResolution.width == 0 || newResolution.height == 0  ) return;

			if( newResolution == Window::resolution ) return;

			Window::resolution = newResolution;

			for( auto& listener : Window::resolutionListeners ) {
				listener->callback(Window::resolution);
			}

			// TODO: Recalculate viewport
		}

	public:
		
		static inline GLFWwindow* window = nullptr;

	};

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


	// =================================================================================================================


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

		//TODO: Remove monitor variable
		GLFWmonitor* monitor;
		monitor = NULL;
		//monitor = glfwGetPrimaryMonitor();
		fullscreen = false;

		glfwWindow = glfwCreateWindow(width, height, this->title.c_str(), monitor, NULL);
		if (!glfwWindow) {
			glfwTerminate();
			throw River::Exception("GLFW initialization error '" + glfwErrorMsg + "'" + " (code: " + std::to_string(glfwErrorCode) + ")");
		}
		NativeWindow::window = glfwWindow;

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


		previousFpsTime = glfwGetTime();

		double mouseX, mouseY;
		glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
		MouseEventController::initialize(mouseX, mouseY);

		glfwSetCursorPosCallback(glfwWindow, glfwMousePosCallback);
		glfwSetScrollCallback(glfwWindow, glfwMouseScrollCallback);
		glfwSetMouseButtonCallback(glfwWindow, glfwMouseButtonCallback);

		glfwSetWindowSizeCallback(glfwWindow, Window::NativeWindow::windowSizeCallback);

		// TODO: Remove this
		// Get supported video modes
		int count;
		const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

		std::cout << "Video modes:" << std::endl;
		for( int i = 0; i < count; i++ ) {
			std::cout << "  " << "Width: " << modes->width << ", " << "Height: " << modes->height << ", " << "Refresh: " << modes->refreshRate << ", " << std::endl;
			modes++;
		}
	}


	Window::~Window() { }


	void Window::setClearColor(Color color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}
	


	void Window::enableFullscreen(const Resolution& resolution) {
		if( resolution.width == 0 || resolution.height == 0 ) {
			throw new InvalidArgumentException("Cannot set window width or height to 0");
		}
		fullscreen = true;

		glfwSetWindowMonitor(NativeWindow::window, glfwGetPrimaryMonitor(), 0, 0, resolution.width, resolution.height, GLFW_DONT_CARE);
		
		int width, height;
		glfwGetWindowSize(NativeWindow::window, &width, &height);
	
		int fWidth, fHeight;
		glfwGetFramebufferSize(NativeWindow::window, &fWidth, &fHeight);
		glViewport(0, 0, fWidth, fHeight);

		std::cout << "Fullscreen was set to " << width << "x" << height << std::endl;
		std::cout << "Framebuffersize was set to " << fWidth << "x" << fHeight << std::endl;
	}

	
	void Window::disableFullscreen(const Resolution& resolution) {
		if( resolution.width == 0 || resolution.height == 0 ) {
			throw new InvalidArgumentException("Cannot set window width or height to 0");
		}
		fullscreen = false;

		glfwSetWindowMonitor(NativeWindow::window, NULL, 0, 0, resolution.width, resolution.height, GLFW_DONT_CARE);

		center();
	}


	bool Window::isFullscreen() {
		return fullscreen;
	}


	void Window::setResolution(const Resolution& resolution) {
		if( resolution.width == 0 || resolution.height == 0 ) {
			throw new InvalidArgumentException("Cannot set window width or height to 0");
		}
		glfwSetWindowSize(NativeWindow::window, (int)resolution.width, (int)resolution.height);
	}

	
	const Resolution& Window::getResolution() {
		return resolution;
	}


	void Window::addResolutionListener(const ResolutionListener* listener) {
		auto iterator = std::find(resolutionListeners.begin(), resolutionListeners.end(), listener);
		if( iterator != resolutionListeners.end() ) {
			throw new InvalidArgumentException("Resolution listener has already been added");
		}
		resolutionListeners.push_back(listener);
	}

	
	void Window::removeResolutionListener(const ResolutionListener* listener) {
		auto iterator = std::find(resolutionListeners.begin(), resolutionListeners.end(), listener);
		
		if( iterator == resolutionListeners.end() ) {
			throw new InvalidArgumentException("Resolution listener was not found");
		}

		resolutionListeners.erase(iterator);
	}


	void Window::center() {
		if( fullscreen ) return;

		// Retrieve primary monitor info to center window
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* monitorVideoMode = glfwGetVideoMode(monitor);
		int monitorX;
		int monitorY;
		glfwGetMonitorPos(monitor, &monitorX, &monitorY);

		int windowX = monitorX + (monitorVideoMode->width - resolution.width) / 2;
		int windowY = monitorY + (monitorVideoMode->height - resolution.height) / 2;

		glfwSetWindowPos(NativeWindow::window, windowX, windowY);
	}


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
		Window* window = glfwWindowMap[glfwWindow];
		MouseEventController::registerMouseMovement(mouseX-window->width/2.0, mouseY*(-1)+window->height/2.0);
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
