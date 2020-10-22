#include "Layer.h"

namespace River {


	Layer::Layer(Layer* parent)
		: parent(parent)
	{}

	Layer::~Layer() {
		// When a layer is deleted, wit will call its destroy callback
		// and recursively delete its children

		if( destroyCallback != nullptr ) destroyCallback();
		for( auto subLayer : layers ) delete subLayer;
	}


	void Layer::clean() {
		for( auto layer : layersToAdd ) {
			layers.push_back(layer);
			if( layer->createCallback != nullptr )
				layer->createCallback();
			layer->clean();
		}
		layersToAdd.clear();

		for( auto layer : layersToRemove ) {
			auto iterator = std::find(layers.begin(), layers.end(), layer);
			if( iterator == layers.end() )
				continue;
			layers.erase(iterator);
			layer->destroy();
		}
		layersToRemove.clear();

		// Clean sub layers
		for( auto layer : layers ) {
			layer->clean();
		}
	}



	void Layer::setCamera(Camera *camera){
		this->camera = camera;
	}

	Camera* Layer::getCamera(){
		return camera;
	}


	Layer* Layer::pushLayer() {
		Layer* layer = new Layer(this);
		layersToAdd.push_back(layer);
		return layer;
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


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Callback setters


	void Layer::create() {
		if( createCallback != nullptr ) createCallback(); 
	}
	void Layer::onCreate(std::function<void()> callback) {
		createCallback = callback;
	}


	void Layer::update() {
		if( updateCallback != nullptr ) updateCallback();
		for( auto subLayer : layers ) subLayer->update();
	}

	void Layer::onUpdate(std::function<void()> callback) {
		updateCallback = callback;
	}


	void Layer::destroy() {
		// Sort of a special case... It doesn't call the destroy callbakc directly, but
		// removes this layer from its parent (marked for removal), or does nothing if
		// it has no parent. This should only be the case of the root layer, which shouldn't
		// be deleted anyways
		if( parent != nullptr ) parent->removeLayer(this);
	}

	void Layer::onDestroy(std::function<void()> callback) {
		destroyCallback = destroyCallback;
	}


	void Layer::keyEvent(KeyEvent& e) {
		for( auto it = layers.rbegin(); it != layers.rend(); it++ ) {
			if( e.isConsumed() ) break;
			(*it)->keyEvent(e);
		}
		if( keyCallback != nullptr && !e.isConsumed() ) keyCallback(e);
	}

	void Layer::onKeyEvent(std::function<void(KeyEvent&)> callback) {
		keyCallback = callback;
	}


	void Layer::mouseMoveEvent(MouseMoveEvent& e) {
		for( auto it = layers.rbegin(); it != layers.rend(); it++ ) {
			if( e.isConsumed() ) break;
			(*it)->mouseMoveEvent(e);
		}
		if( mouseMoveCallback != nullptr && !e.isConsumed() ) mouseMoveCallback(e);
	}

	void Layer::onMouseMoveEvent(std::function<void(MouseMoveEvent&)> callback) {
		mouseMoveCallback = callback;
	}


	void Layer::mouseScrollEvent(MouseScrollEvent& e) {
		for( auto it = layers.rbegin(); it != layers.rend(); it++ ) {
			if( e.isConsumed() ) break;
			(*it)->mouseScrollEvent(e);
		}
		if( mouseScrollCallback != nullptr && !e.isConsumed() ) mouseScrollCallback(e);
	}

	void Layer::onMouseScrollEvent(std::function<void(MouseScrollEvent&)> callback) {
		mouseScrollCallback = callback;
	}


	void Layer::mouseButtonEvent(MouseButtonEvent& e) {
		for( auto it = layers.rbegin(); it != layers.rend(); it++ ) {
			if( e.isConsumed() ) break;
			(*it)->mouseButtonEvent(e);
		}
		if( mouseButtonCallback != nullptr && !e.isConsumed() ) mouseButtonCallback(e);
	}

	void Layer::onMouseButtonEvent(std::function<void(MouseButtonEvent&)> callback) {
		mouseButtonCallback = callback;
	}
}

