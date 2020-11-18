#pragma once

#include "River/Vendor/RiverECS/ECS.h"
#include "River/Graphics/Texture/Sprite.h"
#include "AnimatedSprite.h"

namespace River::ECS {

	class SpriteAnimationSystem {

	public:
		void update(Domain& domain, double timeStep) {

			domain.forMatchingEntities<AnimatedSprite, Sprite>( [&timeStep](auto& entity, AnimatedSprite* animation, Sprite* sprite) {
				
				if( animation->animation == nullptr ) return;
				if( animation->paused ) return;

				animation->time += timeStep * animation->speed;
				
				animation->time = fmod(animation->time, animation->animation->getTotalDuration());

				// Set sprite to correct frame
				sprite->texture = animation->animation->getFrameAtTime(animation->time);
			});

		}





	};

}

