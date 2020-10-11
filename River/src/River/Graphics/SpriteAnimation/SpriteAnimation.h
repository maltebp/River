#pragma once

#include <cmath>

#include "SpriteSequence.h"
#include "River/Error.h"

namespace River {
	
	// TODO: Split correctly to cpp file
	
	/**
	 * @brief	Holds information about a specific SpriteAnimation
				This includes the speed of the information, and may in the future include more options.
	*/
	class SpriteAnimation {
	public:
		
		SpriteAnimation(float speed, SpriteSequence& sequence) :
			speed(speed),
			sequence(sequence),
			length(speed*sequence.getNumSprites())
		{
			if( speed <= 0 ) throw new InvalidArgumentException("Animation speed must be larger than 0");
		}


		/**
		 * @brief	Returns the animation Texture with the given index in the animation sequence
		*/
		const Texture* getSprite(unsigned int index) {
			return sequence[index];
		}
		

		/**
		 * @return	The Texture at the given time in the animation
		*/
		const Texture* getSpriteAtTime(float time) {
			return sequence[(int) std::fmod(length, time)];
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
			return sequence.getNumSprites();
		}


	private:
		SpriteSequence& sequence;

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


