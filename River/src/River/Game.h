#pragma once

#include <vector>
#include <functional>

#include "Error.h"
#include "Graphics/Window.h"
#include "Layer/Layer.h"

namespace River {

	class Game {
	public:
	
		static void setTitle(const std::string&);

		static void setWindowSize(unsigned int width, unsigned int height);

		static void start();
		
		static void start(std::function<void()> onStart);

		static void exit();

		static double getFps();


		static Window* getWindow();


		static void pushLayer(Layer* layer);

		static void popLayer();

		static void clearLayers();

		static void removeLayer(Layer* layer);





	public:
		static inline Window* window = nullptr;

	private:

		static inline std::vector<Layer*> layers;
		static inline std::vector<Layer*> layersToAdd;
		static inline std::vector<Layer*> layersToRemove;

		static inline std::function<void()> initCallback = nullptr;

		static inline bool started = false;

		static inline std::string title = "River";

		static inline unsigned int windowWidth	= 1280;
		static inline unsigned int windowHeight = 720;
	};
}
