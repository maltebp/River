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

				animation->time += timeStep*animation->speed;
				
				auto animationSpeed = animation->animation->getSpeed();
				while( animation->time >= animationSpeed ) {
					animation->time -= animationSpeed;
					animation->currentFrame++;
					if( animation->currentFrame >= animation->animation->getNumFrames() )
						animation->currentFrame = 0;
				}

				// Set sprite to correct frame
				sprite->texture = animation->animation->getSprite(animation->currentFrame);
			});

		}





	};

}

