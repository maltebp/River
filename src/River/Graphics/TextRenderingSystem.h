#pragma once

#include "Window.h"
#include "Camera/Camera.h"
#include "ImageRenderer.h"
#include "River/Vendor/RiverECS/ECS.h"


namespace River {

	class TextRenderingSystem {
	public:

		// TODO: Remove this at some point
		static void initialize(Window* window);

		static void render(Camera* camera, ECS::Domain& domain);


	private:
		static inline ImageRenderer* imageRenderer = nullptr;

	};

}



