#pragma once

#include "River/Graphics/Camera/Camera.h"

namespace River {
	class Layer {

	protected:
		Layer(){}

		Camera *camera;

		virtual void onInitialization(){}
		virtual void onUpdate(){}
		virtual void onTermination(){}

		void setCamera(Camera *camera);

	public:
		void initialize();
		void update();
		void terminate();


		Camera * getCamera();

		
	};
}


