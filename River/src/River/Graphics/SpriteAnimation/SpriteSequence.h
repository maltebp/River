#pragma once

#include "River/Graphics/Texture/Sprite.h"

namespace River {

	/**
	 * @brief	A list of sprites defining an animation sequence
	*/
	class SpriteSequence {
	public:
		

		/**
		 * @brief	Construct an SpriteSequence from a variable number of Sprites, using variadic templates.
		*/
		template <typename ... Sprites>
		SpriteSequence(Sprites ... sprites) {
			numSprites = sizeof...(Sprites);
			spriteList = new SubTexture*[numSprites];

			// Populate array
			int i = 0;
			(void(spriteList[i++] = sprites), ...); 
		}


		~SpriteSequence() {
			delete[] spriteList;
		}


		const SubTexture* operator[](unsigned int index) {
			return spriteList[index];
		}


		unsigned int getNumSprites() {
			return numSprites;
		}


	private:
		SpriteSequence(const SpriteSequence&) = delete;
		SpriteSequence& operator=(const SpriteSequence&) = delete;

	private:
		SubTexture** spriteList;
		unsigned int numSprites;
	};

}


