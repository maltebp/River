#pragma once

#include <vector>

#include "Camera/Camera.h"
#include "Window.h"

namespace River {
	class Renderer {


	protected:
		Window *window = nullptr;
		Camera *camera = nullptr;
		
		Renderer(Window *window);

	public:
 		void flush();
		virtual void onFlush(){ };
	
		void setCamera(Camera *camera);
	};
}


