#pragma once

#include "River/Vendor/RiverECS/ECS.h"
#include "SpriteAnimation.h"

namespace River::ECS {

	struct AnimatedSprite : Component {

		/**
		 * @brief	The Asset to, which describes the Animation to use */
		SpriteAnimation* animation = nullptr;

		/**
		 * @brief	Percentage speed of the animation	*/
		float speed = 1.0f;

		/**
		 * @brief	Whether or not the animation is paused. If paused, time won't get increased, and it will be paused at the current frame.
		*/
		bool paused = false;

		/**
		 * @brief	The current time of the animation (how far into the animation we are)*/
		float time = 0;
	};

}

