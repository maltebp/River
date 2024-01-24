#pragma once

#include "Window.h"
#include "Camera/Camera.h"
#include "ImageRenderer.h"
#include "River/External/RiverECS/ECS.h"


namespace River {

	class TextRenderingSystem {
	public:

		// TODO: Remove this at some point
		static void initialize();

		static void render(ECS::Domain* domain, Camera* camera);


	private:
		static inline ImageRenderer* imageRenderer = nullptr;

	};

}



