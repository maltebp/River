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

	/**
	 * @brief	Represents a logical layer in the Game. Layers are structure in a hierarchy, where each layer,
	 *			may have a number of children, and each child its own children etc.
	 *			
	 *			Layers are added to the game using the 'pushLayer' methods of the this class or the Game class.
	 *			You may add either "Raw" Layers (no inheritance), or your own specialised Layer by inhereting
	 *			this class.
	*/
	class Layer {
	public:


		/**
		 * @brief	Create and add a new child Layer to this Layer. It's a "raw" Layer, and functionality
		 *			may be added by setting the 'on[Something]' std::functions.
		*/
		Layer* pushLayer() {
			Layer* layer = new Layer();
			layersToAdd.push_back(layer);
			return layer;
		}


		/**
		 * @brief	Create and add a new child Layer to this Layer, of the given template type.
		 *
		 * @tparam L	Class which inherits publicly from this class
		 * @param ...args	Arguments which are passed to the constructor of the class
		 * @return	Pointer to the newly created layer of type L
		*/
		template<typename L, typename ... Args>
		L* pushLayer(Args ... args) {
			static_assert(std::is_convertible<L*, Layer*>::value, "Class must publicly inherit from River::Layer");
			
			L* layer = new L(args...);
			layersToAdd.push_back(layer);
			return layer;
		}


		/**
		 * @brief	Removes a child Layer from this Layer.
		 *			The layer is only marked for deletion, and will only effectively be removed during next "cleaning" of this layer.
		 *
		 * @param layer	The layer to add
		 * @throws River::IllegalArgumentException	If the 'layer' is a nullptr
		*/
		void removeLayer(Layer* layer);


		/**
		 * @brief	Removes all children from this layer by calling the 'removeLayer(..)' on
		 *			each child.
		*/
		void clearLayers();


		/**
		 * @return	The parent Layer of this layer. It may be null, if called before onCreate(..), or if
  		 *			the layer has no parent (is root layer).
		*/
		Layer* getParent();


		/**
		 * @brief	Function called when the Layer has been effectively added to its parent (after the parent has been cleaned)
		*/
		std::function<void()> onCreate = [](){};
		std::function<void()> onUpdate = [](){};
		std::function<void()> onDestroy = [](){};
		
		std::function<void(KeyEvent&)> onKeyEvent = [](auto e){};
		std::function<void(MouseMoveEvent&)> onMouseMoveEvent = [](auto e){};
		std::function<void(MouseScrollEvent&)> onMouseScrollEvent = [](auto e){};
		std::function<void(MouseButtonEvent&)> onMouseButtonEvent = [](auto e){};




	protected:
		Layer() {}
		~Layer();

	private:
		Layer(Layer&) = delete;
		Layer& operator=(Layer& other) = delete;

		/**
		 * @brief	Recursively cleans this layer and all its sub layers
		*/
		void clean();

		// Methods which fire callbacks
		void create();
		void update();
		void destroy();
		void keyEvent(KeyEvent&);
		void mouseMoveEvent(MouseMoveEvent&);
		void mouseScrollEvent(MouseScrollEvent&);
		void mouseButtonEvent(MouseButtonEvent&);


	private:
		Layer* parent = nullptr;

		std::vector<Layer*> layers;
		std::vector<Layer*> layersToAdd;
		std::vector<Layer*> layersToRemove;
	
		friend Game;
	};
}


