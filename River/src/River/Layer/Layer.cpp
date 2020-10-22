#include "Layer.h"

namespace River {
	void Layer::initialize(){
		// TODO: Implement
		onInitialization();
	}


	void Layer::update(){
		// TODO: Implement (like updating ECS)
		onUpdate();
	}

	void Layer::terminate(){
		// TODO: Implement
		onTermination();
	}

	void Layer::setCamera(Camera *camera){
		this->camera = camera;
	}

	Camera* Layer::getCamera(){
		return camera;
	}
}

