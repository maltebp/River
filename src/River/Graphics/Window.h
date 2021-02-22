#pragma once

#include <unordered_map>
#include <vector>
#include <functional>

#include "River/Primitives/Resolution.h"
#include "River/Primitives/Color.h"
#include "River/Event/KeyEvent/KeyEventController.h"
#include "River/Error.h"
#include "River/Graphics/GL.h"


namespace River {

	class Game;

	// TODO: Inform user that the primary monitor is used

	class Window {
	public:
		friend Game;
		struct ResolutionListener; friend ResolutionListener;
		class NativeWindow; friend NativeWindow;
		
		Window(std::string title, unsigned int width, unsigned int height);
		~Window();

		void clear();
		void clearDepth();
		bool shouldClose();
		void close();
		unsigned int getNumTextureSlots();
		std::vector<KeyEvent> getKeyEvents();
		double getFps();
		void setClearColor(Color color);

		// Note:
		//  - User should query the actual resolution with getResolution(..)
		static void setResolution(const Resolution& resolution);

		static const Resolution& getResolution();

		// dont resize within this listener
		static void addResolutionListener(const ResolutionListener* listener);

		// removes first instant
		static void removeResolutionListener(const ResolutionListener* listener);

		// dont resize within this listener
		static void addViewportListener(const ResolutionListener* listener);

		// removes first instant
		static void removeViewportListener(const ResolutionListener* listener);

		static void enableFullscreen(const Resolution& resolution = resolution);

		static void disableFullscreen(const Resolution& resolution = resolution);

		static bool isFullscreen();

		// Centers on primary monitor
		// Has no effect if fullscreen
		static void center();

		struct ResolutionListener {
			ResolutionListener(std::function<void(Resolution)> callback) : callback(callback) {}
			const std::function<void(Resolution)> callback;
		};
		
	private:
		static std::unordered_map<GLFWwindow*, Window*> glfwWindowMap;
		static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void glfwMousePosCallback(GLFWwindow* window, double xpos, double ypos);
		static void glfwMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void glfwMouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mode);

	private:

		static inline Resolution resolution = { 720, 480 };

        GLFWwindow* glfwWindow;
        int width;
        int height;

		double mouseXTransform;
		double mouseYTransform;

        std::string title = "River Window";

		/* The number of texture slots (or units) accessible from the fragment shader */
		int numTextureSlots;

		
		double fps = 0;
		double previousFpsTime;
		unsigned int frameCount;

		Color clearColor;

		KeyEventController keyEventController;

		static inline bool fullscreen = false;

		static inline std::vector<const ResolutionListener*> resolutionListeners;
		static inline std::vector<const ResolutionListener*> viewportListeners;

	};
}


