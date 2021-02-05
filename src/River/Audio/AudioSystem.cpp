
#include "AudioSystem.h"

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include "River.h"
#include "ALUtility.h"

namespace River {


	class Data {
	public:
		static inline ALuint source;
	};

	
	void AudioSystem::initialize() {
		if (initialized) throw new InvalidStateException("Audio system has already been initialized");
		initialized = true;

		// Open default device
		ALCdevice* device = alcOpenDevice(nullptr);
		if (device == nullptr) throw new AudioException("Could not open preffered audio device");

		// Not sure if we should check if eax2.0 exists here
		// The OpenAL programmer guide does, but it does not use the result 
		
		// Open context
		ALCcontext* context = alcCreateContext(device, nullptr);
		alcMakeContextCurrent(context);
		ALUtility::checkContextError(device);
		
		// TODO: Just a test source -> delete this when testing is done
		{
			alGenSources(1, &Data::source);
			ALUtility::checkError();

			alSourcef(Data::source, AL_PITCH, 1);
			alSourcef(Data::source, AL_GAIN, 1.0f);
			///* here's the change, position is X, Y, Z */
			alSource3f(Data::source, AL_VELOCITY, 0, 0, 0);
			alSource3f(Data::source, AL_POSITION, 0, 0, 0);
			alSourcei(Data::source, AL_LOOPING, AL_FALSE);
			ALUtility::checkError();

			// Set listener position
			ALfloat listenerPos[] = { 0, 0, 0 };
			alListenerfv(AL_POSITION, listenerPos);
			ALUtility::checkError();
		}
	}





	void AudioSystem::playAudio(AudioAsset* asset) {

		if (!asset->isStream()) {
			ALuint bufferId = *static_cast<ALuint*>(asset->getData());
			alSourcei(Data::source, AL_BUFFER, bufferId);
			ALUtility::checkError();
			alSourcePlay(Data::source);
			ALUtility::checkError();
		}

		//alSourcei(source, AL_BUFFER, buffer);
	}



	
	

	

}



