#pragma once

#include <functional>

#include "River/Graphics/Camera/Camera.h"
#include "River/Event/MouseEvent/MouseMoveEvent.h"
#include "River/Event/MouseEvent/MouseScrollEvent.h"
#include "River/Event/MouseEvent/MouseButtonEvent.h"
#include "River/Event/KeyEvent/KeyEvent.h"

namespace River {

	// Forward declaring game, so we can friend it
	class Game;

	class Layer {
	public:

		Layer* pushLayer();
		void removeLayer(Layer* layer);
		void clearLayers();

		void onCreate(std::function<void()> callback);
		void onUpdate(std::function<void()> callback);
		void onDestroy(std::function<void()> callback);
		
		void onKeyEvent(std::function<void(KeyEvent&)> callback);
		void onMouseMoveEvent(std::function<void(MouseMoveEvent&)> callback);
		void onMouseScrollEvent(std::function<void(MouseScrollEvent&)> callback);
		void onMouseButtonEvent(std::function<void(MouseButtonEvent&)> callback);

	
		Camera * getCamera();


	protected:

		Camera* camera = nullptr;

		void setCamera(Camera* camera);

		void create();
		void update();
		void destroy();
		void keyEvent(KeyEvent&);
		void mouseMoveEvent(MouseMoveEvent&);
		void mouseScrollEvent(MouseScrollEvent&);
		void mouseButtonEvent(MouseButtonEvent&);


	private:
		friend Game;

		Layer(Layer* parent);
		~Layer();

		Layer(Layer&) = delete;
		Layer& operator=(Layer& other) = delete;

		/**
		 * @brief	Recursively cleans this layer and all its sub layers
		*/
		void clean();

	private:
		std::function<void()> createCallback = nullptr;
		std::function<void()> updateCallback = nullptr;
		std::function<void()> destroyCallback = nullptr;
		
		std::function<void(KeyEvent&)> keyCallback = nullptr;
		std::function<void(MouseMoveEvent&)> mouseMoveCallback = nullptr;
		std::function<void(MouseScrollEvent&)> mouseScrollCallback = nullptr;
		std::function<void(MouseButtonEvent&)> mouseButtonCallback = nullptr;

		Layer* parent = nullptr;

		std::vector<Layer*> layers;
		std::vector<Layer*> layersToAdd;
		std::vector<Layer*> layersToRemove;
	};
}


