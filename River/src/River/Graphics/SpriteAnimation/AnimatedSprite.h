#pragma once

#include "River/Vendor/RiverECS/ECS.h"
#include "SpriteAnimation.h"

namespace River::ECS {

	struct AnimatedSprite : Component {
		SpriteAnimation* animation = nullptr;
		unsigned int currentFrame = 0;
		float speed = 1.0f;
		float time = 0;
		bool paused = false;
	};

}

