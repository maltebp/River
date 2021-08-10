#pragma once

#include <unordered_map>
#include <vector>
#include <functional>

#include "River/External/glm/glm.hpp"
#include "River/Utility/Event.h"
#include "River/Graphics/ResolutionEvent.h"
#include "River/Primitives/Resolution.h"
#include "River/Primitives/Color.h"


namespace River {

	using namespace glm;


	class Game;

	class FrameBuffer;

	/**
	 * @brief	Represents the window used to display the single window that
	 *			the game is displayed within.
	 * 
	 *			Initialization:
	 *			The Window is opened automatically when the game is started,
	 *			but properties (resolution, title etc.) may be initialized before
	 *			and these properties will be applied when it opens.
	 *			
	 *			Viewport vs. resolution:
	 *			The viewport is the actual pixel size of the OpenGL context, and
	 *			may differ from the resolution. As such, the viewport should
	 *			always be used mouse calculations, and world to window coordinate
	 *			conversion
	 * 
	 *			Monitor:
	 *			The Window always uses the device's primary monitor
	*/
	class Window {
		friend Game;
		friend FrameBuffer;
	public:
		
		class NativeWindow; friend NativeWindow;	

		static bool isOpen();

		/**
		 * @brief	Sets the title of the Window (displayed in the Window border)
		*/
		static void setTitle(std::string title);

		static std::string getTitle();
		
		/**
		 * @brief	Request the Window to change to the given size. The actual
		 *			size may differ. In fullscreen mode the resolution is set
         *			to the resolution most closely matching one of the resolutions
		 *			retrieved by querying Screen::getSupportedResolutions().
		 *			
		 *			As such, its recommended to bind resolution dependant logic
		 *			on the resolutionChangedListeners, instead of getResolution().
		 * 
		 * @throws	River::InvalidArgumentException	if either Resolution width or
		 *			height is 0
		*/
		static void setResolution(const Resolution& size);

		static const Resolution& getResolution();

		/**
		 * @brief	Fired when the Window resolution changes.
		 * 
		 * @warning	Do not change the resolution within this listener, as it
		 *			may result in undefined behavior
		*/
		static inline ListenerMap<ResolutionEvent&> resolutionChangedListeners;

		static const Resolution& getViewportResolution();

		static inline ListenerMap<ResolutionEvent&> viewportChangedListeners;

		static void enableFullscreen(const Resolution& windowResolution = resolution);

		static void disableFullscreen(const Resolution& windowResolution = resolution);

		static bool isFullscreen();

	
		/**
		 * @brief	Centers the Window on the primary monitor.
		 *			Has no effect if Window is fullscreen
		*/
		static void center();

		/**
         * @brief	Sets the area within the viewport in which to render. If the size is {0,0} it
         *          will render to the full size of the first color buffer or the depth buffer if
         *          no color buffer is present.
         */
		static void setRenderArea(dvec2 position, Resolution size);

		static void setClearColorValue(Color color);

		static void setClearDepthValue(double value);

		static void clearDepth();

		static double getFps();

		static unsigned int getNumTextureSlots();
		
	private:

		Window() = delete;

		static void open();

		static void invokeEvents();

		static void close();

		static bool shouldClose();

		static void useRenderArea();

		static void beginFrame();

		static void endFrame();

	private:

		static inline	bool opened = false;

		static inline	std::string title = "Game";

		static inline	Resolution resolution = { 800, 600 };
		
		static inline	Resolution viewportResolution = { 0, 0 };

		static inline	bool fullscreen = false;
		
		static inline ListenerInvoker resolutionChangedInvoker = (resolutionChangedListeners);

		static inline ListenerInvoker viewportChangedInvoker = (viewportChangedListeners);

		static inline dvec2 renderAreaPosition = {0,0};

		static inline Resolution renderAreaSize = {0,0};

		/* The number of texture slots (or units) accessible from the fragment shader */
		static inline int numTextureSlots;

		static inline double fps = 0;

		static inline double previousFpsTime;

		static inline unsigned int frameCount = 0;

		static inline Color clearColorValue = Colors::BLACK;

		static inline double clearDepthValue = 1.0;
 
	};
}


