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
		std::vector<Layer*> layersToAdd;
		std::vector<Layer*> layersToRemove;


		std::vector<Layer*> overlays;
		// TODO: Add removal and adding of overlays like layers
		
		static Game* game;

	protected:
		River::Window* window;

	public:
		static Game* getGame();


		Game(std::string title);
		virtual ~Game();

		/**
			@throws River::Exception Thrown if an error occurs in the game loop
		*/
		void start();

		void pushLayer(Layer* layer);

		void popLayer();

		void clearLayers();

		void removeLayer(Layer* layer);

		void pushOverlay(Layer* layer);

		void popOverlay();

		void clearOverlays();

		void exit();


		double getFps();


		virtual void onInitialization() {}

		Window* getWindow();
	};
}
