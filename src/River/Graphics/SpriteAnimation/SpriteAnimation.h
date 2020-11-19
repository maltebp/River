#pragma once

#include "River/Vendor/RiverECS/ECS.h"
#include "TextureAnimation.h"

namespace River::ECS {

	/**
	 * @brief	Describes an animation sequence for the Sprite component.
	 *			Used by the SpriteAnimationSystem to animate and update Entities
	 *			with a SpriteAnimation and Sprite component.
	*/
	struct SpriteAnimation : Component {

		/**
		 * @brief	The Asset to, which describes the Animation to use */
		TextureAnimation* animation = nullptr;

		/**
		 * @brief	Percentage speed of the animation	*/
		float speed = 1.0f;

		/**
		 * @brief	Whether or not the animation is paused. If paused, time won't get increased, and it will be paused at the current frame.
		*/
		bool paused = false;

		/**
		 * @brief	The current time of the animation (how far into the animation we are). Can be set to 0
		 *			at any point to reset the animation back to the start*/
		float time = 0;
	};

}

