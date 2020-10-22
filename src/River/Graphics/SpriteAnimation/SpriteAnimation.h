#pragma once

#include "River/Error.h"

namespace River {
	

	/**
	 * @brief	Holds information about a specific SpriteAnimation
				This includes the speed of the information, and may in the future include more options.
	*/
	class SpriteAnimation {
	public:

		/**
		 * @brief	Defines a new Sprite animation, by a sequence of textures and an animation speed.
		 *
		 * @param	speed	Seconds between each texture frame
		 * @param	sprites	The textures to construct the Sprite from (must be of type River::Texture*)
		*/
		template<typename ... Sprites>
		SpriteAnimation(float speed, Sprites ... textures) : speed(speed) {

			// Asserting sprite parameters 
			static_assert(sizeof...(Sprites) > 0, "SpriteAnimation must receieve at least one texture for its sequence");
			static_assert(std::conjunction<std::is_same<River::Texture*, Sprites>...>::value, "Sprites must be of type River::Texture*");

			if( speed <= 0 ) throw new InvalidArgumentException("Animation speed must be larger than 0");

			numSprites = sizeof...(Sprites);
			spriteList = new Texture * [numSprites];

			// Populate array
			int i = 0;
			(void(spriteList[i++] = textures), ...); // Fold expression
		
			length = speed * numSprites;
		}


		/**
		 * @brief	Returns the animation Texture with the given index in the animation sequence
		*/
		const Texture* getSprite(unsigned int index) {
			return spriteList[index];
		}
		

		/**
		 * @return	The Texture at the given time in the animation
		*/
		const Texture* getSpriteAtTime(float time) {
			return spriteList[(int) std::fmod(length, time)];
		}


		/**
		 *	Returns the time length of each frame in seconds
		 */
		float getSpeed() {
			return speed;
		}


		/**
		 *	Get number of frames (Sprites) in the animation
		 */
		unsigned int getLength() {
			return length;
		}

		
		unsigned int getNumFrames() {
			return numSprites;
		}


	private:

		Texture** spriteList;
		unsigned int numSprites;

		/**
		 * @brief	Time between each frame in seconds
		*/
		float speed;

		/**
		 * @brief	The time length of the animation (speed * number of sprites in sequence)
		*/
		float length;

	};
}


