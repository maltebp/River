#pragma once

#include <functional>

#include "River/Graphics/Camera/Camera.h"
#include "River/Mouse/MouseMoveEvent.h"
#include "River/Mouse/MouseScrollEvent.h"
#include "River/Mouse/MouseButtonEvent.h"


namespace River {

	// Forward declaring game, so we can friend it
	class Game;


	/**
	 * @brief	Represents a logical layer in the Game.
	 *			Layers are structure in a hierarchy, where each layer, may have a number of children,
	 *			and each child its own children etc.
	 *			
	 *			Layers are added to Layers or to the Game using the 'pushLayer' methods in either of those classes.
	*/
	class Layer {
	public:


		/**
		 * @brief	Create and add a new child Layer to this Layer
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
		 * @brief	Marks a child of this layer for removal and deletion. All children of the given Layer will be
		 *			marked recursively as well.
		 *			
		 *			The actual removal/deletion will take place, when this Layer is being cleaned.
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
		 * @brief	Sets an action function to be called immediately after the matching virtual function.
		*/
		void onCreate(std::function<void()> action);

		/**
		 * @brief	Sets an action function to be called immediately after the matching virtual function.
		*/
		void onUpdate(std::function<void()> action);

		/**
		 * @brief	Sets an action function to be called immediately after the matching virtual function.
		*/
		void onDestroy(std::function<void()> action);
		

	protected:

		Layer() {}

		virtual ~Layer();

		/**
		 * @brief	Method called when the Layer has been effectively added to its parent (after the parent has been cleaned)
		*/
		virtual void onCreate(){}

		virtual void onUpdate(){}

		virtual void onDestroy(){}


	private:

		Layer(Layer&) = delete;

		Layer& operator=(Layer& other) = delete;

		/**
		 * @brief	Recursively cleans this layer and all its sub layers
		*/
		void clean();

		void update();

		void destroy();


	private:

		Layer* parent = nullptr;

		std::vector<Layer*> layers;

		std::vector<Layer*> layersToAdd;

		std::vector<Layer*> layersToRemove;


		// Actions - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		std::function<void()> onCreateAction = [](){};

		std::function<void()> onUpdateAction = [](){};

		std::function<void()> onDestroyAction = [](){};

		//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		friend Game;
	};
}


