#pragma once

#include "River/Graphics/ImageRenderer.h"
#include "River/Graphics/Window.h"
#include "River/Vendor/RiverECS/ECS.h"


namespace River {

	class SpriteRenderingSystem {
	public:
		
		// TODO: Remove this at some point
		static void initialize();

		static void render(ECS::Domain* domain, Camera* camera);


	private:
		static ImageRenderer* imageRenderer;

	};

}



