#pragma once

#include "River/Asset/Asset.h"
#include "River/Asset/AssetCreator.h"

namespace River {
	
	class AudioAsset : public Asset {
	public:

		/// <returns>A pointer to the native file handle</returns>
		void* getFile();

		/// <returns>A pointer to the native audio data</returns>
		void* getData();

		/// <returns>True if this AudioAsset is a stream</returns>
		bool isStream();

	protected:
		virtual void onLoad() override;
		virtual void onUnload() override;

	private:
		AudioAsset();
		~AudioAsset();

	private:
		// Whether or not this audio file should be streamed
		bool stream = false;

		// Byte size of each sample
		size_t sampleSize = 0;

		uint64_t numSamples = 0;

		// How many samples should be "played" per second
		// Determines the playback speed of the audio clip
		uint32_t sampleRate = 0;
	
		std::string filePath = "";
	
		// Native data handle
		void* nativeData = nullptr;


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Creator
	public: 
		class Creator : public AssetCreator<AudioAsset::Creator, AudioAsset>{
			friend class AudioAsset;
		private:
			Creator(const std::string& filePath);

		public:

			/**
			* @brief	Sets whether or not this AudioAsset should be fully loaded and stored into memory
			*			when loaded, of if it should be streamed only when needed.
			*/
			void setStreamed(bool toggle);

		};

		/**
		* @brief	Returns a an AudioAssetCreator, to define an AudioAsset
		*/
		static Creator create(const std::string& filePath) {
			return Creator(filePath);
		}
	};
}


