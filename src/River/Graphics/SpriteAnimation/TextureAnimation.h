#pragma once

#include "River/Graphics/Texture/Texture.h"
#include "River/Error.h"
#include "River/Asset/Asset.h"
#include "River/Asset/AssetCollection.h"


namespace River {
	

	/**
	 * @brief	Asset to define a sequence of Textures as frames in an animation. The speed of the animation is
	 *			defined by setting an unique duration for each texture, or a uniform duration for all
	 *			textures.
	*/
	class TextureAnimation : public Asset {
	public:

		/**
		 * @brief	Returns the Texture with the given index in the animation sequence
		*/
		const Texture* getFrame(unsigned int index) const {
			return frames[index];
		}
		
		/**
		 * @return	The frame (Texture*) at the given time in the animatiom. If the time goes beyond the animation's total duration, nullptr is returned.
		*/
		const Texture* getFrameAtTime(float time) const {
			if( time < 0 ) return nullptr;
			if( time > totalDuration ) return nullptr;

			float durationSum = 0;

			for( int i=0; i<durations.size(); i++ ) {
				durationSum += durations[i];
				if( time < durationSum ) return frames[i];
			}

			/* Notes on this lookup:
			 * This "calculation" has to happen each frame, for each AnimatedSprite component
			 * active in the system. However, since the list is expected to be relatively small
			 * and it's sequential memory (vector), I expect it has a very small performance impact,
			 * compared to the complexity of implementing a faster lookup system.
			*/

			return nullptr;
		}

		/**
		 *	@returns The duration of a particular frame given its index
		 */
		float getFrameDuration(unsigned int frameIndex) const {
			if( frameIndex >= durations.size() )
				throw new InvalidArgumentException("Frame index is out of bounds");
			return durations[frameIndex];
		}

		/**
		 *	@returns The sum of all frame's duration (the total length of the animation)
		 */
		float getTotalDuration() const {
			return totalDuration;
		}
		
		/**
		 *	@returns The number of frames (Textures) in the animation
		 */
		unsigned int getNumFrames() const {
			return frames.size();
		}


		virtual void onLoad() override {
			for( auto frame : frames ) {
				frame->load();
			}
		}


		virtual void onUnload() override {
			for( auto frame : frames ) {
				frame->unload();
			}
		}




	private:
		TextureAnimation() {}

		TextureAnimation(const TextureAnimation&) = delete;
		TextureAnimation& operator=(const TextureAnimation&) = delete;

		void cleanDurationData() {
			recalculateTotalDuration();
						
		}

		// Utility function to calculate total duration
		void recalculateTotalDuration() {
			totalDuration = 0;
			for( auto duration : durations )
				totalDuration += duration;
		}

		// Utility function to recalculate frame time lookup
		void recalculateTimeLookup() {
			timeLookup.clear();

			unsigned int numLookups = 2 * frames.size();
			timeLookupStep = totalDuration / numLookups;
			float lookupTime = 0;
			float frameEndTime = 0;
			for( int i = 0; i < frames.size(); i++ ) {
				frameEndTime += durations[i];
				while( lookupTime <= frameEndTime ) {
					timeLookup.push_back(frames[i]);
					lookupTime += timeLookupStep;
				}
			}
		}


	private:
	
		std::vector<Texture*> frames;
		std::vector<float> durations;

		// Cached value of sum of frame durations
		float totalDuration;

		float timeLookupStep; 
		std::vector<Texture*> timeLookup;


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Creator
	public:
		
		class Creator {
			friend class TextureAnimation;

			bool finished = false;
			TextureAnimation* animation;
			AssetCollection* collection = nullptr;

			template<typename ... Frames>
			Creator(Frames ... frames) {
				// Asserting sprite parameters 
				static_assert(sizeof...(Frames) > 0, "TextureAnimation must receieve at least one texture for its sequence");
				static_assert(std::conjunction<std::is_same<River::Texture*, Frames>...>::value, "Sprites must be of type River::Texture*");

				animation = new TextureAnimation();

				// Populate array
				(void(animation->frames.push_back(frames)), ...); // Fold expression

				animation->durations.assign(animation->frames.size(), 0.25);
				
				animation->recalculateTotalDuration();
			}


		public:


			/**
			 * @brief	Sets the duration for each frame of the Animation
			 * @param ... durations	Duration of the each texture in same order as the frames
			*/
			template<typename ... Durations>
			Creator& setFrameDurations(Durations ... durations) {
				static_assert(std::conjunction<std::is_same<float, Durations>...>::value, "Sprite animation durations must be of type float");

				unsigned int numTimes = sizeof...(Durations);

				if( numTimes != animation->frames.size() )
					throw new InvalidArgumentException("Number of animation durations does not match number of frames in TextureAnimation");

				animation->durations.clear();
				(void(animation->durations.push_back(durations)), ...);

				animation->recalculateTotalDuration();

				return *this;
			}

			/**
			 * @brief	Sets the total duration of the Sprite, by uniformly distributing the duration
			 *			across all the frames.
			 * @param time	The duration in seconds
			*/
			Creator& setDuration(float duration) {
				if( !(duration > 0) )
					throw new InvalidArgumentException("Animation duration of TextureAnimation must be larger than 0");
				std::fill(animation->durations.begin(), animation->durations.end(), duration / animation->frames.size());

				animation->recalculateTotalDuration();

				return *this;
			}

			/**
			 * @brief	Sets the duration for a single frame
			 * @param textureIndex	The index of the frame (as passed in the creation function)
			 * @param duration	Duration in seconds
			*/
			Creator& setFrameDuration(unsigned int frameIndex, float duration) {
				if( !(duration > 0) )
					throw new InvalidArgumentException("Animation duration of TextureAnimation must be larger than 0");
				if( !(frameIndex < animation->durations.size()) )
					throw new InvalidArgumentException("Frame index is out of bounds for TextureAnimation");
				animation->durations[frameIndex] = duration;

				animation->recalculateTotalDuration();

				return *this;
			}


			/**
			 * @brief	Sets the collection of Assets that this TextureAnimation asset belongs to
			*/
			Creator& setAssetCollection(AssetCollection* collection) {
				this->collection = collection;

				return *this;
			}


			/**
			 * @brief	Finishes the creation of the Texture
			 * @return	Pointer to the dynamically allocated Texture
			*/
			TextureAnimation* finish() {
				if( finished )
					throw new InvalidStateException("TextureAnimation::Creator has already finished");

				if( collection != nullptr ) collection->add(animation);

				finished = true;
				return animation;
			}
					
		};


		/**
		 * @brief	Initialize a Creator object for a TextureAnimation by a variable number of Texture pointers
		 *			to build the animation from. The order of the textures is dependent on the order for the 
		 *			arguments
		 * @param	frames	The Textures to construct the animation from from (must be of type River::Texture*)
		 */
		template<typename ... Frames>
		static Creator create(Frames ... frames) {
			return Creator(frames...);
		}


	};
}


