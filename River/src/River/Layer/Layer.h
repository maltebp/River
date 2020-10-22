#pragma once

#include "River/Graphics/Camera/Camera.h"
#include "River/Event/MouseEvent/MouseMoveEvent.h"
#include "River/Event/KeyEvent/KeyEvent.h"

namespace River {
	class Layer {

		// TODO: Make layer receive game or window at creation

	protected:

		Camera *camera = nullptr;

		virtual void onInitialization(){}
		virtual void onUpdate(){}
		virtual void onTermination(){}

		void setCamera(Camera *camera);

	public:
		void initialize();
		void update();
		void terminate();

		virtual void onKeyEvent(KeyEvent&) {}
		virtual void onMouseMoveEvent(MouseMoveEvent&) {}


		Camera * getCamera();

		
	};
}


