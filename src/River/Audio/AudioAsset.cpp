#include "AudioAsset.h"


#define DR_WAV_IMPLEMENTATION
#include "River/Vendor/dr_wav/dr_wav.h"
#include "ALUtility.h"

namespace River {

	struct NativeData {
		drwav* file = nullptr;
		ALuint samplesBuffer = 0;
	};
	
	AudioAsset::AudioAsset() { }

	AudioAsset::~AudioAsset() { }

	void AudioAsset::onLoad() {
		auto nativeData = new NativeData();

		// Open file (does not load data)
		drwav file;
		{
			bool success = drwav_init_file(&file, filePath.c_str(), NULL);
			if (!success)
				// doing some more manual loading of the wav file could
				// provide us with a more accurate error message, but that
				// is left out for now
				throw new AssetException("Asset could not be loaded for some reason");
		
			if (file.channels != 1 && file.channels != 2)
				// Not sure if this is ever important
				throw new InvalidAssetException("Audio asset file must be either mono or stereo");

			if (file.bitsPerSample != 16)
				throw new InvalidAssetException("Audio asset file must have sample size of 16 bits");

			numSamples = file.totalPCMFrameCount;
			sampleSize = file.bitsPerSample / 8;
			sampleRate = file.sampleRate;
		}

		if (stream) {
			// Stor< file for streaming later
			*nativeData->file = file;
		}

		// Pre-load audio data into OpenAL buffer
		else {
			
			// Read file data
			int16_t* data = new int16_t[numSamples * file.channels];
			if (data == nullptr) throw new Exception("Malloc failed");
			drwav_read_pcm_frames_s16(&file, file.totalPCMFrameCount, static_cast<drwav_int16*>(data));

			// Convert stereo to mono (in-place)
			if (file.channels == 2) {
				for (size_t i = 0; i < numSamples; i++)
					// Upcast then downcast to prevent overflow
					data[i] = static_cast<int16_t>((static_cast<int32_t>(data[i * 2]) + static_cast<int32_t>(data[i * 2 + 1])) * 0.5);
			}

			{ // Copy to OpenAL buffer
				ALuint bufferId;

				alGenBuffers(1, &bufferId);
				ALUtility::checkErrors();

				alBufferData(bufferId, AL_FORMAT_MONO16, data, static_cast<ALsizei>(numSamples)*2, static_cast<ALsizei>(sampleRate));
				ALUtility::checkErrors();

				nativeData->samplesBuffer = bufferId;
			}

			// The file is not needed for non-streamed audio
			drwav_uninit(&file);
			delete[] data;
		}

		// Assign the audio native data
		this->nativeData = static_cast<void*>(nativeData);

	}


	void AudioAsset::onUnload() {
		// TODO: Free native Data
		// TODO: Free file
		// TODO: Free buffer id
		// TODO: Unhook from playback system (not decided how this is to be done)
	}


	void* AudioAsset::getFile() {
		if (!isLoaded())
			throw new AssetNotLoaded();
		if (!stream)
			throw new InvalidAssetStateException("AudioAsset is not a stream");
		return static_cast<NativeData*>(nativeData)->file;
	}


	void* AudioAsset::getData() {
		if (!isLoaded())
			throw new AssetNotLoaded();
		if (stream)
			throw new InvalidAssetStateException("AudioAsset is a stream");
		return &(static_cast<NativeData*>(nativeData)->samplesBuffer);
	}


	double AudioAsset::getLength() {
		if (!isLoaded())
			throw new AssetNotLoaded();
		return (double)((long double)numSamples / (long double)sampleRate);
	}


	bool AudioAsset::isStream() {
		return stream;
	}


	unsigned int AudioAsset::getPriority() {
		return priority;
	}



	// - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Creator

	AudioAsset::Creator::Creator(const std::string& filePath) {
		// For now, the library only supports wave files
		if (filePath.substr(filePath.find_last_of(".") + 1) != "wav")
			throw new InvalidArgumentException(filePath);
		
		asset = new AudioAsset();
		asset->filePath = filePath;
	}


	void AudioAsset::Creator::setStreamed(bool toggle) {
		asset->stream = toggle;
	}

	
	void AudioAsset::Creator::setPriority(unsigned int priority) {
		asset->priority = priority;
	}
}