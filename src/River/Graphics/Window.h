#pragma once

#include <unordered_map>
#include <vector>
#include <functional>

#include "River/Event/Event.h"
#include "River/Graphics/ResolutionEvent.h"
#include "River/Primitives/Resolution.h"
#include "River/Primitives/Color.h"


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


	private:

		static inline	bool opened = false;

		static inline	std::string title = "Game";

		static inline	Resolution resolution = { 800, 600 };
		
		static inline	Resolution viewportResolution = { 0, 0 };

		static inline	bool fullscreen = false;
		
		static inline ListenerInvoker resolutionChangedInvoker = (resolutionChangedListeners);

		static inline ListenerInvoker viewportChangedInvoker = (viewportChangedListeners);

		/* The number of texture slots (or units) accessible from the fragment shader */
		static inline int numTextureSlots;

		static inline double fps = 0;

		static inline double previousFpsTime;

		static inline unsigned int frameCount = 0;

		static inline Color clearColor;

	};
}


