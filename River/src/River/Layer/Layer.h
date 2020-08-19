#pragma once

#include "River/Event/KeyEvent/KeyEvent.h"
#include "River/Graphics/Camera/Camera.h"

namespace River {
	class Layer {

	protected:
		Layer(){}

		Camera *camera;

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


