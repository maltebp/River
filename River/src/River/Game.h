#pragma once

#include <vector>

#include "Error.h"
#include "Graphics/Window.h"
#include "Layer/Layer.h"

namespace River {

	class Game {
		
	private:
		std::string title;
		std::vector<Layer*> layers;
		std::vector<Layer*> overlays;
		
	protected:
		River::Window* window;

	public:
		Game(std::string title);
		virtual ~Game();

		/**
			@throws River::Exception Thrown if an error occurs in the game loop
		*/
		void start();

		void pushLayer(Layer* layer);

		void popLayer();

		void clearLayers();

		void pushOverlay(Layer* layer);

		void popOverlay();

		void clearOverlays();

		virtual void onInitialization() {}

		Window* getWindow();
	};
}
