#include "Window.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "Screen.h"
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

			resolutionChanged = true;
		}


		static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
			Resolution newViewport((unsigned int)width, (unsigned int)height);

			if( newViewport.width < 0 || newViewport.height < 0 ) {
				// Just a safety precaution (not sure if it's ever possible)
				throw new Exception("GLFW window size callback passed negative width or height");
			}

			// Window size is set to 0x0 when it gets minimized (both fullscreen and windowed mode)
			if( newViewport.width == 0 || newViewport.height == 0 ) return;

			if( newViewport == Window::viewportResolution ) return;

			Window::viewportResolution = newViewport;

			glViewport(0, 0, (GLsizei)viewportResolution.width, (GLsizei)viewportResolution.height);

			viewportChanged = true;
		}	


		static void errorCallback(int errCode, const char* errStr) {
			std::stringstream stream;
			stream << "0x"
				<< std::setfill('0') << std::setw(8)
				<< std::hex << errCode;
			std::string codeStr(stream.str());
			throw new Exception("GLFW error: '" + std::string(errStr) + "' (code: " + codeStr + ")");
		}


	public:
		
		static inline GLFWwindow* window = nullptr;

		static inline bool resolutionChanged = false;

		static inline bool viewportChanged = false;

	};

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

	//Window::Window(std::string title, unsigned int width, unsigned int height) {
	//	this->title = title;

	//	// TODO: Make the user able to adjust this
	//	this->width = width;
	//	this->height = height;

	//	glfwSetErrorCallback(glfwErrorCallback);

	//	// Initialize glfw
	//	if (!glfwInit()) {   
	//		throw River::Exception("GLFW initialization error '" + glfwErrorMsg + "'" + " (code: " + std::to_string(glfwErrorCode) + ")");
	//	}


	//	// TODO: Figure out to handle sampling per texture
	//	//glfwWindowHint(GLFW_SAMPLES, 16);

	//	//TODO: Remove monitor variable
	//	GLFWmonitor* monitor;
	//	monitor = NULL;
	//	//monitor = glfwGetPrimaryMonitor();
	//	fullscreen = false;

	//	glfwWindow = glfwCreateWindow(width, height, this->title.c_str(), monitor, NULL);
	//	if (!glfwWindow) {
	//		glfwTerminate();
	//		throw River::Exception("GLFW initialization error '" + glfwErrorMsg + "'" + " (code: " + std::to_string(glfwErrorCode) + ")");
	//	}
	//	NativeWindow::window = glfwWindow;

	//	glfwSetKeyCallback(glfwWindow, glfwKeyCallback);


	//	glfwWindowMap[glfwWindow] = this;

	//	// Set window to current
	//	glfwMakeContextCurrent(glfwWindow);

	//	// Get number of texture slots
	//	GL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &numTextureSlots));
	//
	//	// TODO: Set this up a proper place
	//	GL(
	//		glEnable(GL_ALPHA_TEST);
	//		glEnable(GL_BLEND);
	//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//		glEnable(GL_DEPTH_TEST);
	//		glDepthFunc(GL_LESS); // TODO: This is redundandt (GL_LESS is default)
	//	);

	//	// This enable binary alpha blending
	//	glAlphaFunc(GL_GREATER, 0);


	//	previousFpsTime = glfwGetTime();

	//	double mouseX, mouseY;
	//	glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
	//	MouseEventController::initialize(mouseX, mouseY);

	//	glfwSetCursorPosCallback(glfwWindow, glfwMousePosCallback);
	//	glfwSetScrollCallback(glfwWindow, glfwMouseScrollCallback);
	//	glfwSetMouseButtonCallback(glfwWindow, glfwMouseButtonCallback);

	//	glfwSetWindowSizeCallback(glfwWindow, Window::NativeWindow::windowSizeCallback);
	//	glfwSetFramebufferSizeCallback(glfwWindow, Window::NativeWindow::framebufferSizeCallback);

	//	// TODO: Remove this
	//	printf("\nScreen resolutions:\n");
	//	Resolution currentResolution = Screen::getResolution();
	//	for( auto& resolution : Screen::getSupportedResolutions() ) {
	//		std::cout << "  " << resolution.width << "x" << resolution.height;
	//		if( resolution == currentResolution ) {
	//			std::cout << "  (Current)";
	//		}
	//		std::cout << std::endl;
	//	}
	//}


	//Window::~Window() { }


	void Window::open() {
		if( opened ) {
			throw InvalidStateException("Window is already open");
		}

		opened = true;

		bool initialized = glfwInit();
		if( !initialized ) {
			// Error is triggered from callback
			return;
		}
	
		NativeWindow::window = glfwCreateWindow(
				resolution.width,
				resolution.height,
				title.c_str(),
				fullscreen ? glfwGetPrimaryMonitor() : NULL,
				NULL
			);

		if( !NativeWindow::window ) {
			glfwTerminate();
			// Error is triggered from callback
			return;
		}

		if( !fullscreen ) {
			center();
		}


		// Set window to current
		glfwMakeContextCurrent(NativeWindow::window);

		// TODO: Set this up a proper place (GL stuff)

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

		glfwSetKeyCallback(NativeWindow::window, glfwKeyCallback);

		double mouseX, mouseY;
		glfwGetCursorPos(NativeWindow::window, &mouseX, &mouseY);
		MouseEventController::initialize(mouseX, mouseY);

		glfwSetCursorPosCallback(NativeWindow::window, glfwMousePosCallback);
		glfwSetScrollCallback(NativeWindow::window, glfwMouseScrollCallback);
		glfwSetMouseButtonCallback(NativeWindow::window, glfwMouseButtonCallback);

		glfwSetWindowSizeCallback(NativeWindow::window, Window::NativeWindow::windowSizeCallback);
		glfwSetFramebufferSizeCallback(NativeWindow::window, Window::NativeWindow::framebufferSizeCallback);

	}
	

	bool Window::isOpen() {
		return opened;
	}


	void Window::setResolution(const Resolution& resolution) {
		if( resolution.width == 0 || resolution.height == 0 ) {
			throw new InvalidArgumentException("Cannot set window width or height to 0");
		}

		if( !opened ) {
			// If window is 1, the resolution will be updated through the callback
			Window::resolution = resolution;
			return;
		}

		glfwSetWindowSize(NativeWindow::window, (int)resolution.width, (int)resolution.height);
	}

	
	const Resolution& Window::getResolution() {
		return resolution;
	}


	const Resolution& Window::getViewportResolution() {
		return viewportResolution;
	}


	void Window::enableFullscreen(const Resolution& resolution) {
		if( resolution.width == 0 || resolution.height == 0 ) {
			throw new InvalidArgumentException("Window width and height must be larger than 0");
		}

		fullscreen = true;

		if( !opened ) {
			// If window is open, the resolution will be updated through the callback
			Window::resolution = resolution;
			return;
		}

		// TODO: CLean up below

		glfwSetWindowMonitor(NativeWindow::window, glfwGetPrimaryMonitor(), 0, 0, resolution.width, resolution.height, GLFW_DONT_CARE);

		int width, height;
		glfwGetWindowSize(NativeWindow::window, &width, &height);

		int fWidth, fHeight;
		glfwGetFramebufferSize(NativeWindow::window, &fWidth, &fHeight);
		//glViewport(0, 0, fWidth, fHeight);

		std::cout << "Fullscreen was set to " << width << "x" << height << std::endl;
		std::cout << "Framebuffersize was set to " << fWidth << "x" << fHeight << std::endl;
	}


	void Window::disableFullscreen(const Resolution& resolution) {
		if( resolution.width == 0 || resolution.height == 0 ) {
			throw new InvalidArgumentException("Cannot set window width or height to 0");
		}

		fullscreen = false;
		if( !opened ) {
			// If window is open, the resolution will be updated through the callback
			Window::resolution = resolution;
			return;
		}

		glfwSetWindowMonitor(NativeWindow::window, NULL, 0, 0, resolution.width, resolution.height, GLFW_DONT_CARE);

		center();
	}


	bool Window::isFullscreen() {
		return fullscreen;
	}


	void Window::setTitle(std::string title) {
		Window::title = title;
		
		if( !opened ) return;

		glfwSetWindowTitle(NativeWindow::window, title.c_str());
	}

	
	std::string Window::getTitle() {
		return title;
	}


	void Window::setClearColor(Color color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}


	void Window::center() {
		if( fullscreen ) return;

		// TODO: Utilize screen class

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





	void Window::close() {
		glfwSetWindowShouldClose(NativeWindow::window, GL_TRUE);
	}


	bool Window::shouldClose() {
		return glfwWindowShouldClose(NativeWindow::window);
	}

	
	

	// TODO: This should not be here
	unsigned int Window::getNumTextureSlots() {
		if (numTextureSlots == 0)
			throw River::Exception("Number of texture slots is 0");
		return numTextureSlots;
	}


	void Window::invokeEvents() {

		// Fire resolution changed event
		if( NativeWindow::resolutionChanged ) {
			ResolutionEvent event(resolution);
			for( auto& listener : resolutionChangedInvoker.listeners ) {
				listener.second(event);
				if( event.isConsumed() ) break;
			}
			NativeWindow::resolutionChanged = false;
		}

		// Fire viewport changed event
		if( NativeWindow::viewportChanged ) {
			ResolutionEvent event(viewportResolution);
			for( auto& listener : viewportChangedInvoker.listeners ) {
				listener.second(event);
				if( event.isConsumed() ) break;
			}
			NativeWindow::viewportChanged = false;
		}

		glfwPollEvents();

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

		mouseX *= viewportResolution.width / (double)resolution.width;
		mouseY *= viewportResolution.height / (double)resolution.height;
		
		mouseX -= viewportResolution.width / 2.0;
		mouseY *= -1;
		mouseY += viewportResolution.height / 2.0;

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

		glfwSwapBuffers(NativeWindow::window);
		GL(glDepthMask(GL_TRUE)); // We must be able to write to the depth buffer in order to clear the bit
		GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}


	void Window::clearDepth() {
		GL(glClear(GL_DEPTH_BUFFER_BIT));
	}

}
