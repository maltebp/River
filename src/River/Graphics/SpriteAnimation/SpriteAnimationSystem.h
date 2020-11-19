#pragma once

#include "SpriteAnimation.h"
#include "River/Vendor/RiverECS/ECS.h"
#include "River/Graphics/Texture/Sprite.h"

namespace River {

	class SpriteAnimationSystem {

	public:

		static void update(ECS::Domain* domain, double timeStep) {

			domain->forMatchingEntities<ECS::SpriteAnimation, ECS::Sprite>( [&timeStep](auto& entity, ECS::SpriteAnimation* animation, ECS::Sprite* sprite) {
				
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

