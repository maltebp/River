#include "Renderer.h"

#include <iostream>

namespace River {
	Renderer::Renderer(River::Window *window) {
		this->window = window;
	}

	void Renderer::flush(){
		onFlush();
	}

	void Renderer::setCamera(Camera *camera){
		this->camera = camera;
	}
}


