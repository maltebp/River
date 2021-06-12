#pragma once

#include "River/Graphics/Texture/Texture.h"
#include "River/Error.h"

#include "River/Asset/Asset.h"
#include "River/Asset/AssetCollection.h"
#include "River/Asset/AssetCreator.h"


namespace River {
	

	/**
	 * @brief	Asset to define a sequence of Textures as frames in an animation. The speed of the animation is
	 *			defined by setting an unique duration for each texture, or a uniform duration for all
	 *			textures.
	*/
	class TextureAnimation : public Asset {
		struct FrameData;
	public:

		/**
		 * @brief	Returns the Texture with the given index in the animation sequence
		*/
		const Texture* getFrame(unsigned int index) const;
		
		/**
		 * @return	The frame (Texture*) at the given time in the animatiom. If the time goes beyond the animation's total duration, nullptr is returned.
		*/
		const Texture* getFrameAtTime(float time) const;

		/**
		 *	@returns The duration of a particular frame given its index
		 */
		float getFrameDuration(unsigned int frameIndex) const;

		/**
		 *	@returns The sum of all frame's duration (the total length of the animation)
		 */
		float getTotalDuration() const;
		
		/**
		 *	@returns The number of frames (Textures) in the animation
		 */
		unsigned int getNumFrames() const;


		virtual void onLoad() override;


		virtual void onUnload() override;

	public:

		static inline const float DEFAULT_FRAME_LENGTH = 0.25; // Seconds		

	private:

		TextureAnimation() {}

		TextureAnimation(const TextureAnimation&) = delete;

		TextureAnimation& operator=(const TextureAnimation&) = delete;

		// Utility function to calculate total duration
		void recalculateTotalDuration();

	private:
	
		std::vector<FrameData> frames;

		float totalDuration;

	private:

		struct FrameData {
		public:

			Texture* texture;

			float duration;

			// Whether the Texture is owned by this Animation
			bool dedicatedTexture;

		};

	public:

		struct FrameParameter {
		public:

			/**
			 * @brief	Define a Frame from a non-dedicated Texture source.
			 */
			FrameParameter(Texture* texture);

			/**
			 * @brief	Define a Frame from a dedicated Texture source.
			 */
			FrameParameter(Texture::Creator& textureCreator);

			/**
			 * @brief	Define a Frame from a dedicated Image source.
			 */
			FrameParameter(Image::Creator& imageCreator);		
			
		public:

			bool dedicatedTexture = false;

			Texture* texture = nullptr;

		};


		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// Creator
		class Creator : public AssetCreator<Creator, TextureAnimation> {
			friend class TextureAnimation;
		public:

			/**
			 * @brief	Sets the total duration of the Sprite, by uniformly distributing the duration
			 *			across all the frames.
			 * @param time	The duration in seconds
			*/
			Creator& setDuration(float duration);

			/**
			 * @brief	Sets the duration for a single frame
			 * @param textureIndex	The index of the frame (as passed in the creation function)
			 * @param duration	Duration in seconds
			*/
			Creator& setFrameDuration(unsigned int frameIndex, float duration);

			operator TextureAnimation*();

		protected:

			void onFinish() override;

		private:

			Creator(std::vector<FrameParameter> frames);
					
		};


		/**
		 * @brief	Construct a new TextureAnimation from a series of frames.
		 * 			
		 * 			Default frame length is DEFAULT_FRAME_LENGTH seconds.
		 * 
		 * @param frames 	Vector of frames to use for the animation either sourced from non-dedicated
		 * 					Textures or dedicated Textures or Images. The order of the frames in the
		 * 					animation matches that of this list.
		 */
		static Creator create(std::vector<FrameParameter> frames);

	};
}


