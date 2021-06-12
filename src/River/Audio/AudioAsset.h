#pragma once

#include "River/Asset/Asset.h"
#include "River/Asset/AssetCreator.h"

namespace River {
	
	class AudioAsset : public Asset {
	public:

		static inline const unsigned int DEFAULT_PRIORITY = 10;

		/// <returns>A pointer to the native audio data</returns>
		void* getData();

		/**
	     *	@returns  The length of this AudioAsset in seconds
		 */
		double getLength();

		/// <returns>Returns the priority of this sound asset</returns>
		unsigned int getPriority();

	protected:
		virtual void onLoad() override;
		virtual void onUnload() override;

	private:
		AudioAsset() { }
		~AudioAsset() { }

	private:
		// Byte size of each sample
		size_t sampleSize = 0;

		uint64_t numSamples = 0;

		// How many samples should be "played" per second
		// Determines the playback speed of the audio clip
		uint32_t sampleRate = 0;
	
		std::string filePath = "";
	
		// Native data handle
		void* nativeData = nullptr;

		unsigned int priority = DEFAULT_PRIORITY;


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Creator
	public: 
 		class Creator : public AssetCreator<AudioAsset::Creator, AudioAsset>{
			friend class AudioAsset;
		private:
			Creator(const std::string& filePath);

		public:

			/**
			 * @brief	Sets the playing priority of AudioAsset. This will influence (not determine), whether
			 *			audio instances of this asset will be played if not enough sound channels are present
			 *			to play all audio instances.
			 */
			void setPriority(unsigned int);

			operator AudioAsset*();

		};

		/**
		* @brief	Returns a an AudioAssetCreator, to define an AudioAsset
		*/
		static Creator create(const std::string& filePath) {
			return Creator(filePath);
		}
	};
}


