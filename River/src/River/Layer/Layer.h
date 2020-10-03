#pragma once

#include "River/Event/KeyEvent/KeyEvent.h"
#include "River/Graphics/Camera/Camera.h"

namespace River {
	class Layer {

		// TODO: Make layer receive game or window at creation

	protected:

		Camera *camera = nullptr;

		virtual void onInitialization(){}
		virtual void onUpdate(){}
		virtual void onTermination(){}
		virtual void onKeyEvent(KeyEvent &e){}

		void setCamera(Camera *camera);

	public:
		void initialize();
		void update();
		void terminate();
		void keyEvent(KeyEvent &e);


		Camera * getCamera();

		
	};
}


