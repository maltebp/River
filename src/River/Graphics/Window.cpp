#include "Window.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "GL.h"
#include "Screen.h"
#include "River/Mouse/MouseController.h"
#include "River/Keyboard/KeyboardController.h"


namespace River {

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


	class Window::NativeWindow {
	public:
		
		// Callback for GLFW window size change function
		static void windowSizeCallback(GLFWwindow* window, int width, int height) {
			updateWindowSize(width, height);
		}

		static void updateWindowSize(int width, int height) {

			if( width < 0 || height < 0 ) {
				// Just a safety precaution (not sure if it's ever possible)
				throw new Exception("GLFW window size callback passed negative width or height");
			}

			Resolution newResolution((unsigned int)width, (unsigned int)height);

			// Window size is set to 0x0 when it gets minimized (both fullscreen and windowed mode)
			if( newResolution.width == 0 || newResolution.height == 0 ) return;

			if( newResolution == Window::resolution ) return;

			Window::resolution = newResolution;

			// Will cause event to be fired
			resolutionChanged = true;
		}


		static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
			updateViewport(width, height);
		}

		static void updateViewport(int width, int height) {
		
			if( width < 0 || height < 0 ) {
				// Just a safety precaution (not sure if it's ever possible)
				throw new Exception("GLFW window size callback passed negative width or height");
			}

			Resolution newViewport((unsigned int)width, (unsigned int)height);

			std::cout << "New viewport: " << width << "x" << height << std::endl;

			// Window size is set to 0x0 when it gets minimized (both fullscreen and windowed mode)
			if( newViewport.width == 0 || newViewport.height == 0 ) return;

			if( newViewport == Window::viewportResolution ) return;

			Window::viewportResolution = newViewport;

			glViewport(0, 0, (GLsizei)viewportResolution.width, (GLsizei)viewportResolution.height);

			// Will cause event to be fired
			viewportChanged = true;
		}


		static void keyCallback(GLFWwindow* glfwWindow, int glfwKey, int scancode, int glfwAction, int mods) {
			Key key = (Key)glfwKey;

			switch( glfwAction ) {
			case GLFW_PRESS:
				KeyboardController::registerKeyDown(key);
				break;
			case GLFW_RELEASE:
				KeyboardController::registerKeyUp(key);
				break;
			case GLFW_REPEAT:
				return; // We don't use this
			default:
				// TODO: Implement correct logging
				std::cout << "WARNING: GLFW action '" << glfwAction << "' is not handled!" << std::endl;
				return;
			}

		}


		static void characterCallback(GLFWwindow* glfwWindow, unsigned int codepoint) {
			char32_t character = (char32_t)codepoint;
			KeyboardController::registerCharacterEvent(character);
		}


		static void mousePosCallback(GLFWwindow* glfwWindow, double mouseX, double mouseY) {

			mouseX *= Window::viewportResolution.width / (double)Window::resolution.width;
			mouseY *= Window::viewportResolution.height / (double)Window::resolution.height;

			mouseX -= Window::viewportResolution.width / 2.0;
			mouseY *= -1;
			mouseY += Window::viewportResolution.height / 2.0;

			MouseController::registerMouseMovement(mouseX, mouseY);
		}


		static void mouseScrollCallback(GLFWwindow* glfwWindow, double xOffset, double yOffset) {
			MouseController::registerMouseScroll(yOffset);
			// yOffset is the classic scroll direction, while xOffset is also used for controllers,
			// which are not supported in this framework yet
		}


		static void mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int modifierBits) {
			// Modifier bits are unused (ALT, SHIFT etc...), as they are not important for games

			if( action == GLFW_PRESS ) {
				MouseController::registerButtonDown(glfwConvertMouseButton(button));
				return;
			}

			if( action == GLFW_RELEASE ) {
				MouseController::registerButtonUp(glfwConvertMouseButton(button));
				return;
			}

			// TODO: Log warning
			std::cout << "WARNING - Unknown mouse action code: " << action << std::endl;
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


	// =================================================================================================================


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

		// Get number of texture slots
		GL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &numTextureSlots));

		GL(
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS); // Note: This is redundandt (GL_LESS is default)
		);

		// This enable binary alpha blending
		glAlphaFunc(GL_GREATER, 0);

		previousFpsTime = glfwGetTime();

		glfwSetKeyCallback(NativeWindow::window, NativeWindow::keyCallback);
		glfwSetCharCallback(NativeWindow::window, NativeWindow::characterCallback);

		glfwSetCursorPosCallback(NativeWindow::window, NativeWindow::mousePosCallback);
		glfwSetScrollCallback(NativeWindow::window, NativeWindow::mouseScrollCallback);
		glfwSetMouseButtonCallback(NativeWindow::window, NativeWindow::mouseButtonCallback);

		glfwSetWindowSizeCallback(NativeWindow::window, Window::NativeWindow::windowSizeCallback);
		glfwSetFramebufferSizeCallback(NativeWindow::window, Window::NativeWindow::framebufferSizeCallback);

		// Initialize window size
		int windowWidth, windowHeight;
		glfwGetWindowSize(NativeWindow::window, &windowWidth, &windowHeight);
		NativeWindow::updateViewport(windowWidth, windowHeight);

		// Initialize viewport
		int framebufferWidth, framebufferHeight;
		glfwGetFramebufferSize(NativeWindow::window, &framebufferWidth, &framebufferHeight);
		NativeWindow::updateViewport(framebufferWidth, framebufferHeight);
	
		// Initialize mouse position
		double mouseX, mouseY;
		glfwGetCursorPos(NativeWindow::window, &mouseX, &mouseY);
		MouseController::initialize(mouseX, mouseY);
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

		glfwSetWindowMonitor(NativeWindow::window, glfwGetPrimaryMonitor(), 0, 0, resolution.width, resolution.height, GLFW_DONT_CARE);

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


		Resolution monitorResolution = Screen::getResolution();

		// Retrieve primary monitor info to center window
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		int monitorX;
		int monitorY;
		glfwGetMonitorPos(monitor, &monitorX, &monitorY);

		int windowX = monitorX + (monitorResolution.width - resolution.width) / 2;
		int windowY = monitorY + (monitorResolution.height - resolution.height) / 2;

		glfwSetWindowPos(NativeWindow::window, windowX, windowY);
	}


	void Window::close() {
		glfwSetWindowShouldClose(NativeWindow::window, GL_TRUE);
	}


	bool Window::shouldClose() {
		return glfwWindowShouldClose(NativeWindow::window);
	}


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
