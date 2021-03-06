#include "River/pch.h"

#include "Layer.h"

namespace River {


	Layer::~Layer() {
		// When a layer is deleted, it will call its destroy callback
		// and recursively delete its children

		onDestroy();
		onDestroyAction();
		for( auto subLayer : layers ) delete subLayer;
	}


	void Layer::clean() {
		for( auto layer : layersToAdd ) {
			layer->parent = this;
			layers.push_back(layer);
			layer->onCreate();
			layer->onCreateAction();
			layer->clean();
		}
		layersToAdd.clear();

		for( auto layer : layersToRemove ) {
			auto iterator = std::find(layers.begin(), layers.end(), layer);
			if( iterator == layers.end() )
				continue;
			layers.erase(iterator);
			delete layer;
		}
		layersToRemove.clear();

		// Clean sub layers
		for( auto layer : layers ) {
			layer->clean();
		}
	}


	void Layer::removeLayer(Layer* layer) {
		if( layer == nullptr )
			throw new InvalidArgumentException("Layer cannot be null");
		layersToRemove.push_back(layer);
	}


	void Layer::clearLayers() {
		for( auto layer : layers )
			layersToRemove.push_back(layer);
		for( auto layer : layersToAdd )
			layersToRemove.push_back(layer);
		// Not just removing from the layersToAdd, to maintain the "illusion" that layers
		// are created when you call pushLayer
	}


	Layer* Layer::getParent() {
		return parent;
	}


	void Layer::update() {
		onUpdate();
		onUpdateAction(); 
		for( auto subLayer : layers ) subLayer->update();
	}


	void Layer::destroy() {
		// Sort of a special case... It doesn't call the destroy callbakc directly, but
		// removes this layer from its parent (marked for removal), or does nothing if
		// it has no parent. This should only be the case of the root layer, which shouldn't
		// be deleted anyways
		if( parent != nullptr ) parent->removeLayer(this);
	}


	/**
	 * @brief	Sets an action function to be called immediately after the matching virtual function.
	 */
	void Layer::onCreate(std::function<void()> action) {
		if( action == nullptr ) throw InvalidArgumentException("Action must not be nullptr");
		onCreateAction = action;
	}

	/**
	 * @brief	Sets an action function to be called immediately after the matching virtual function.
	 */
	void Layer::onUpdate(std::function<void()> action) {
		if( action == nullptr ) throw InvalidArgumentException("Action must not be nullptr");
		onUpdateAction = action;
	}

	/**
	 * @brief	Sets an action function to be called immediately after the matching virtual function.
	 */
	void Layer::onDestroy(std::function<void()> action) {
		if( action == nullptr ) throw InvalidArgumentException("Action must not be nullptr");
		onDestroyAction = action;
	}

}

