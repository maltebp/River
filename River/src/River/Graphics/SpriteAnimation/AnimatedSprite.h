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

	// TODO: Consider if AnimatedSprite should have its own Sprite* and be drawn seperately for speed increase
	// This will probably cause duplicate information in the Sprite and this component

}

