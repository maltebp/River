#pragma once

#include "River/Graphics/ImageRenderer.h"
#include "River/Graphics/Window.h"
#include "River/Vendor/RiverECS/ECS.h"


/*
	Algorithm:

		Render:
			


*/



namespace River {

	class SpriteRenderingSystem {
	public:

		
		// TODO: Remove this at some point
		static void initialize(Window* window);


		static void render(Camera* camera, ECS::Domain& domain);


		/*static void renderOpaque(ECS::Domain& domain) {
		
		}*/



		/*static void renderTransparent(ECS::Domain& domain) {

		}*/



	private:
		static ImageRenderer* imageRenderer;

	};

}



