#include "TextRenderingSystem.h"

#include <algorithm>

#include "Texture/Sprite.h"
#include "Transform.h"
#include "Text/Text.h"

#include "River/Error.h"


namespace River {


	void TextRenderingSystem::initialize(Window* window) {
		if( imageRenderer != nullptr )
			throw new AlreadyInitializedException("TextRenderingSystem");
		imageRenderer = new ImageRenderer(window);
	}



	void TextRenderingSystem::render(ECS::Domain* domain, Camera* camera) {
		imageRenderer->setCamera(camera);

		// Just to hold components for entity when sorting
		struct TextEntity {
			ECS::Entity* entity;
			ECS::Transform* transform;
			ECS::Text* text;

			TextEntity(ECS::Entity* entity, ECS::Transform* transform, ECS::Text* text)
				:	entity(entity), transform(transform), text(text)
			{}
		};

		// Get text entities and add them to list
		std::vector<TextEntity> textEntities;
		domain->forMatchingEntities<ECS::Text, ECS::Transform>([&](ECS::Entity* entity, ECS::Text* text, ECS::Transform* transform) {
			textEntities.emplace_back(entity, transform, text);
		});


		// Sort the partially transparent entities
		std::stable_sort(textEntities.begin(), textEntities.end(),
			[](const TextEntity& a, const TextEntity& b) -> bool {
				return a.transform->depth > b.transform->depth;
			}
		);

		imageRenderer->enableBlending();
		for( auto& entity : textEntities ) {
			auto& text = entity.text;
			auto& transform = entity.transform;

			FontInstance* fontInstance = text->font->getFontInstance(text->size);
			FontInstance::TextSize textSize = fontInstance->calculateTextSize(text->text);

			float x = transform->x;
			float y = transform->y;
			float z = transform->depth;

			if( text->alignment.isHCenter() )
				x -= textSize.width / 2.0;

			if( text->alignment.isRight() )
				x -= textSize.width;

			if( text->alignment.isVCenter() )
				y -= fontInstance->getHeight() / 4;

			if( text->alignment.isTop() )
				y -= fontInstance->getHeight() / 1.5;


			bool first = true;
			for( const char& c : text->text ) {
				const FontInstance::Glyph& glyph = fontInstance->getGlyph(c);

				if( glyph.texture != nullptr ) {
					// If texture is nullptr, then the glyph has no image (i.e. it's a space)

					float glyphX = x + glyph.texture->getWidth() / 2.0;
					if( first ) first = false;
					else glyphX += glyph.bearingX;

					float glyphY = y + glyph.bearingY - glyph.texture->getHeight() / 2.0;

					imageRenderer->drawSprite(
						glyph.texture,
						{
							glyphX, glyphY, z,
							(float)glyph.texture->getWidth(), (float)glyph.texture->getHeight(),
							0,
							text->color.a, { text->color.r, text->color.g, text->color.b, 1.0 }
						}
					);
				}

				x += glyph.advance;
			}
		}

		imageRenderer->flush();
	
	}

}