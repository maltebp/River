#include "Renderer.h"


namespace River {
	Renderer::Renderer(River::Window* window) {
		this->window = window;
	}

	void Renderer::flush(){
		onFlush();
	}
}


