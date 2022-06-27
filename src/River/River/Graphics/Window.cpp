#include "River/pch.h"

#include "Window.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "glm.hpp"

#include "GL.h"
#include "River/External/imgui/imgui.h"
#include "River/External/imgui/imgui_impl_glfw.h"
#include "River/External/imgui/imgui_impl_opengl3.h"
#include "River/Game.h"
#include "Screen.h"
#include "River/Mouse/MouseController.h"
#include "River/Keyboard/KeyboardController.h"
#include "River/Graphics/FrameBuffer.h"


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

			// Window size is set to 0x0 when it gets minimized (both fullscreen and windowed mode)
			if( newViewport.width == 0 || newViewport.height == 0 ) return;

			if( newViewport == Window::viewportResolution ) return;

			Window::viewportResolution = newViewport;

			if( FrameBuffer::getCurrent() == nullptr) {
				useRenderArea();
			}

			// Will cause event to be fired
			viewportChanged = true;
		}


		static void keyCallback(GLFWwindow* glfwWindow, int glfwKey, int scancode, int glfwAction, int mods) {
			if( Game::isImGuiEnabled() && ImGui::GetIO().WantCaptureKeyboard ) {
				return;
			}

			KeyboardKey key = (KeyboardKey)glfwKey;

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
			if( Game::isImGuiEnabled() && ImGui::GetIO().WantTextInput ) {
				return;
			}
			char32_t character = (char32_t)codepoint;
			KeyboardController::registerCharacterEvent(character);
		}


		static void mousePosCallback(GLFWwindow* glfwWindow, double mouseX, double mouseY) {
			if( Game::isImGuiEnabled() && ImGui::GetIO().WantCaptureMouse ) {
				return;
			}
			mouseX -= Window::viewportResolution.width / 2.0;
			mouseY -= Window::viewportResolution.height / 2.0;
			mouseY *= -1;

			// Scale to viewport coordinates
			mouseX *= (double)Window::resolution.width / Window::viewportResolution.width;
			mouseY *= (double)Window::resolution.height / Window::viewportResolution.height;

			MouseController::registerMouseMovement(mouseX, mouseY);
		}


		static void mouseScrollCallback(GLFWwindow* glfwWindow, double xOffset, double yOffset) {
			if( Game::isImGuiEnabled() && ImGui::GetIO().WantCaptureMouse ) {
				return;
			}
			MouseController::registerMouseScroll(yOffset);
			// yOffset is the classic scroll direction, while xOffset is also used for controllers,
			// which are not supported in this framework yet
		}


		static void mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int modifierBits) {
			if( Game::isImGuiEnabled() && ImGui::GetIO().WantCaptureMouse ) {
				return;
			}

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

		// glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_SAMPLES, 4);

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

		// Set window to current
		glfwMakeContextCurrent(NativeWindow::window);

		// Initialize GLEW
		const GLenum glewResult = glewInit();
		if( glewResult != GLEW_OK ) {
			std::stringstream msgStream;
			msgStream << "GLEW initialization error '" << glewGetErrorString(glewResult) << "'";
			throw Exception(msgStream.str());
		}

	
 
		if( Game::isImGuiEnabled() ) {
			IMGUI_CHECKVERSION();
    		ImGui::CreateContext();

			// The default config for ImGui. Can be changed be the user
			// at the Game startup
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
			io.ConfigViewportsNoAutoMerge = true;

			ImGui_ImplGlfw_InitForOpenGL(NativeWindow::window, true);
    		ImGui_ImplOpenGL3_Init("#version 130");
		}
	
		if( !fullscreen ) {
			center();
		}

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

		if( mainViewportEnabled ) {
			mainViewport = new MainViewport({(unsigned int)framebufferWidth, (unsigned int)framebufferHeight});
			mainViewportRenderer = new GLTextureRenderer();
		}

	}


	void Window::update(std::function<void()> updateCallback, std::function<void()> imGuiCallback) {
		
		glfwPollEvents();

		glfwMakeContextCurrent(NativeWindow::window);

		// Clear the actual viewport
		GL(glDepthMask(GL_TRUE)); // We must be able to write to the depth buffer in order to clear the bit
		GL(glClearColor(0, 0, 0, 1.0f));
		GL(glClearDepth(clearDepthValue));
		GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ));

		if( mainViewportEnabled ) {
			mainViewport->setResolution(viewportResolution);
			mainViewport->bindFrameBuffer();
			GL(glDepthMask(GL_TRUE)); // We must be able to write to the depth buffer in order to clear the bit
			GL(glClearColor(clearColorValue.r, clearColorValue.g, clearColorValue.b, clearColorValue.a));
			GL(glClearDepth(clearDepthValue));
			GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ));
			mainViewport->unbindFrameBuffer();	
		}

		updateCallback();

		// Render the main viewport's framebuffer
		if( mainViewportEnabled ) {
			mainViewportRenderer->render(mainViewport->frameBuffer->getColorBufferImage(0));
		}

		// Start the Dear ImGui frame
		if( Game::isImGuiEnabled() ) {
			ImGui_ImplOpenGL3_NewFrame();
        	ImGui_ImplGlfw_NewFrame();

			ImGui::NewFrame();

			imGuiCallback();

			ImGui::Render();
        	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			 // Update and Render additional Platform Windows
        	// (Platform functions may change the current OpenGL context
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
        
		glfwMakeContextCurrent(NativeWindow::window);
		glfwSwapBuffers(NativeWindow::window);

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


	void Window::disableMainViewport() {
		mainViewportEnabled = false;
	}

	
	MainViewport* Window::getMainViewport() {
		if( !mainViewportEnabled ) {
			throw new InvalidStateException("Main viewport has been disabled");
		}
		return mainViewport;
	}


	void Window::setRenderArea(dvec2 position, Resolution size) {
		renderAreaPosition = position;
		renderAreaSize = size;
		if( FrameBuffer::getCurrent() == nullptr) {
			useRenderArea();
		}
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
			
			// TODO: Remove this
			ResolutionEvent event(viewportResolution);
			for( auto& listener : viewportChangedInvoker.listeners ) {
				listener.second(event);
				if( event.isConsumed() ) break;
			}
			NativeWindow::viewportChanged = false;
		}

	}


	double Window::getFps() {
		return fps;
	}


	void Window::setClearColorValue(Color color) {
		clearColorValue = color;
	}

	
	void Window::setClearDepthValue(double depth) {
		clearDepthValue = glm::clamp(depth, 0.0 , 1.0);
	}


	void Window::useRenderArea() {
        if( renderAreaSize != Resolution(0,0) ) {
            GL(glViewport(
                (int)renderAreaPosition.x, (int)renderAreaPosition.y,
                 (GLsizei)renderAreaSize.width, (GLsizei)renderAreaSize.height
            ));
        }
        else {
            GL(glViewport(0,0, (GLsizei)viewportResolution.width, (GLsizei)viewportResolution.height));
        }
	}

}
