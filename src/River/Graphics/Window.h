#pragma once

#include <unordered_map>
#include <vector>
#include <functional>

#include "River/Event/Event.h"
#include "River/Graphics/ResolutionEvent.h"
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
		class NativeWindow; friend NativeWindow;	
		
		

		static bool isOpen();

		static void setTitle(std::string title);

		static std::string getTitle();
		
		// Note:
		//  - User should query the actual resolution with getResolution(..)
		static void setResolution(const Resolution& size);

		static const Resolution& getResolution();

		// dont resize within this listener
		static inline ListenerMap<ResolutionEvent&> resolutionChangedListeners;

		// removes first instant
		static const Resolution& getViewportResolution();

		static inline ListenerMap<ResolutionEvent&> viewportChangedListeners;

		static void enableFullscreen(const Resolution& windowResolution = resolution);

		static void disableFullscreen(const Resolution& windowResolution = resolution);

		static bool isFullscreen();

		// Centers on primary monitor
		// Has no effect if fullscreen
		static void center();

		// TODO: Should be seperated from window class
		static std::vector<KeyEvent> getKeyEvents();

		// TODO: Below functions should be moved to seperate "graphics context" class
		static void setClearColor(Color color);

		static void clear();

		static void clearDepth();

		static double getFps();

		static unsigned int getNumTextureSlots();


		
	private:

		Window() = delete;

		static void open();

		static void invokeEvents();

		static void close();

		static bool shouldClose();

		static std::unordered_map<GLFWwindow*, Window*> glfwWindowMap;
		static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void glfwMousePosCallback(GLFWwindow* window, double xpos, double ypos);
		static void glfwMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void glfwMouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mode);

	private:

		static inline std::string title = "Game";

		static inline Resolution resolution = { 800, 600 };

		static inline double viewportMinRatio = 0;
		static inline double viewportMaxRatio = 0;

		static inline bool opened = false;

		// TODO: Set this on startup
		static inline Resolution viewportResolution = { 0, 0 };

		static inline double mouseXTransform;
		static inline double mouseYTransform;

        //std::string title = "River Window";

		/* The number of texture slots (or units) accessible from the fragment shader */
		static inline int numTextureSlots;

		
		static inline double fps = 0;
		static inline double previousFpsTime;
		static inline unsigned int frameCount = 0;

		static inline Color clearColor;

		static inline KeyEventController keyEventController;

		static inline bool fullscreen = false;

		static inline ListenerInvoker resolutionChangedInvoker = (resolutionChangedListeners);

		static inline ListenerInvoker viewportChangedInvoker = (viewportChangedListeners);

	};
}


