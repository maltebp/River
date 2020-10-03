#pragma once

#include "River/Vendor/RiverECS/ECS.h"

#include "SubTexture.h"

namespace River::ECS {

	struct Sprite : Component {
		// TODO: Remove position from this component
		float x = 0;
		float y = 0; 
		const SubTexture* texture = nullptr;

	};

}