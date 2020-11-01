#include "SpriteRenderingSystem.h"

#include <iostream> // TODO: Remove this
#include <algorithm>

#include "Texture/Sprite.h"
#include "Transform.h"

#include "River/Error.h"


namespace River {

	ImageRenderer* SpriteRenderingSystem::imageRenderer = nullptr;


	void SpriteRenderingSystem::initialize(Window* window) {
		if( imageRenderer != nullptr )
			throw new AlreadyInitializedException("SpriteRenderingSystem");
		imageRenderer = new ImageRenderer(window);
	}

	

	void SpriteRenderingSystem::render(Camera* camera, ECS::Domain& domain) {
		imageRenderer->setCamera(camera);

		// Just to hold data for transparent entities
		struct TransparentEntity {
			ECS::Entity* entity;
			ECS::Transform* transform;
			ECS::Sprite* sprite;
			TransparentEntity(ECS::Entity* entity, ECS::Transform* transform, ECS::Sprite* sprite) :
				entity(entity), transform(transform), sprite(sprite) { }
		};

		std::vector<TransparentEntity> transparentEntities;

		// Draw opaque entities and set aside transparent ones
		imageRenderer->disableBlending();
		domain.forEachEntity<ECS::Sprite, ECS::Transform>([&](ECS::Entity* entity, ECS::Sprite* sprite, ECS::Transform* transform) {
			std::cout << "Rendering sprite!" << std::endl;

			if( sprite->isPartiallyTransparent() ) {
				transparentEntities.emplace_back(entity, transform, sprite);
			} else {
				imageRenderer->drawSprite(
					sprite->texture,
					{
						(float)transform->x,
						(float)transform->y,
						(float)transform->depth,
						400, // TODO: What's happening with this width / height?
						400, 
						0,
						sprite->opacity,
						sprite->tint
					}
				);
			}
		});


		// Sort the partially transparent entities
		std::stable_sort(transparentEntities.begin(), transparentEntities.end(),
			[](const TransparentEntity& a, const TransparentEntity& b) -> bool {
				return a.transform->depth > b.transform->depth;
			}
		);

		imageRenderer->flush();

		imageRenderer->enableBlending();
		for( auto& transparentEntity : transparentEntities ) {
			auto& sprite = transparentEntity.sprite;
			auto& transform = transparentEntity.transform;
			imageRenderer->drawSprite(
				sprite->texture,
				{
					(float)transform->x,
					(float)transform->y,
					(float)transform->depth,
					400,
					400,
					0,
					sprite->opacity,
					sprite->tint
				}
			);
		}


		imageRenderer->flush();
	}


}