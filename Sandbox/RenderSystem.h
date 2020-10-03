#pragma once

#include <River.h>

class RenderSystem {

public:
	void update(River::ECS::Domain& domain, River::ImageRenderer* renderer) {
		
		domain.forEachEntity<River::ECS::Sprite>([&](River::ECS::Entity* entity, River::ECS::Sprite* sprite) {


			if( sprite->texture != nullptr )
				renderer->drawSprite(sprite->texture, { sprite->x, sprite->y, 0, 400, 400, 0, River::Colors::WHITE });


		});

	}

};