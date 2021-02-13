#include "AudioSystem.h"

#include <vector>
#include <stack>
#include <algorithm>

#include "River.h"
#include "ALUtility.h"


namespace River {


	// Static AL data used by the AudioSystem
	// Not placed in header to abstract OpenAL away from end user
	class ALData{
	public:
		// The number of AL sources to create. This is in accordance with
		// the expected minimum number of sounds that may play at the same
		// time
		static inline const int NUM_SOURCES = 16;

		static inline std::stack<ALuint*> freeSources;

		static inline ALuint allSources[NUM_SOURCES];
	};


	void AudioSystem::initialize() {
		if (initialized) throw new InvalidStateException("Audio system has already been initialized");
		initialized = true;

		// Open default device
		ALCdevice* device = alcOpenDevice(nullptr);
		if (device == nullptr) throw new AudioException("Could not open preffered audio device");

		// Not sure if we should check if eax2.0 exists here
		// The OpenAL programming guide does, but the return
		// result is never 

		// Open context
		ALCcontext* context = alcCreateContext(device, nullptr);
		alcMakeContextCurrent(context);
		ALUtility::checkContextError(device);

		// Set attenuation model
		alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
		ALUtility::checkErrors();
		
		// Generate sources
		for (int i = 0; i < ALData::NUM_SOURCES; i++) {
			ALuint sourceId;
			alGenSources(1, &sourceId);
			ALUtility::checkErrors();
			ALData::allSources[i] = sourceId;
			ALData::freeSources.push(ALData::allSources+i);
		}
	}


	void AudioSystem::playAudio(AudioInstance* audio) {
		if (audio->playing) return;
		audio->playing = true;
		playingInstances.push_back(audio);
	}


	void AudioSystem::stopAudio(AudioInstance* audio) {
		if (!audio->playing) return;

		if ( audio->isActive() ) {
			deactivateInstance(audio);
		}

		playingInstances.erase(std::find(playingInstances.begin(), playingInstances.end(), audio));
	}


	void AudioSystem::setMasterVolume(double volume){
		if( volume < 0 )
			throw new InvalidArgumentException("Master volume may not be less than 0");
		masterVolume = volume;
		alListenerf(AL_GAIN, (ALfloat)masterVolume);
		ALUtility::checkErrors();
	}


	double AudioSystem::getMasterVolume(){
		return masterVolume;
	}


	void AudioSystem::update(double time) {
		if (!initialized) throw new InvalidStateException("Audio system has not been initialized");

		if (playingInstances.size() == 0) return;

		// Reset number of audio instances for assets
		// (used for heuristic calculation)
		for (auto& it : assetInstanceCount)
			it.second = 0;
		
		// Update time and heuristics
		std::vector<AudioInstance*> finishedInstances;
		for (auto instance : playingInstances) {
			if (instance->paused) continue;

			bool finished = false;

			if( !instance->isActive() ) {
				// Not active: Manually update time
				instance->currentTime += time * instance->speed;
				if (instance->currentTime > instance->asset->getLength()) {
					if (!instance->looping)
						finished = true;
					else
						instance->currentTime = std::fmod(instance->currentTime, instance->asset->getLength());
				}
			}
			else {

				// Use AL to check if finished
				ALuint sourceId = *static_cast<ALuint*>(instance->nativeObject);
				ALenum state;
				alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
				ALUtility::checkErrors();
				if (state == AL_STOPPED)
					finished = true;
			}

			// Check if finished
			if (finished) {
				finishedInstances.push_back(instance);
				continue;
			}

			// Calculate heuristic
			calculateHeuristic(instance);		
		}

		// Remove finished instances
		for (auto instance : finishedInstances) {
			auto instanceIterator = std::find(playingInstances.begin(), playingInstances.end(), instance);
			AudioInstance* instance = *instanceIterator;
			if( instance->isActive() )
				deactivateInstance(instance);
			playingInstances.erase(instanceIterator);
			if( instance->onFinishCallback != nullptr )
				instance->onFinishCallback(instance);
		}

		// Sort instance with heuristic
		// Sorting in new list, as a audioInstances, is in the order
		// they have been "played"
		std::vector<AudioInstance*> sortedInstances(playingInstances);
		std::sort(sortedInstances.begin(), sortedInstances.end(), [](AudioInstance* a1, AudioInstance* a2) {
			return a1->heuristic > a2->heuristic;
		});

		// Make sure the first 16 instances are active
		int activationCount = 0;
		for (AudioInstance* instance : sortedInstances) {

			// Deactivate and skip instances that are paused, or there
			// is not room for
			if (instance->paused || activationCount >= ALData::NUM_SOURCES) {
				if (instance->isActive())
					deactivateInstance(instance);
				continue;
			}

			if (!instance->isActive())
				activateInstance(instance);

			activationCount++;
		}
	}


	double AudioSystem::calculateHeuristic(AudioInstance* audioInstance) {
		/*	An AudioInstance's heuristic is used to determine whether or not
		*   it should be played, when there are more AudioInstances present
		*   than there are OpenAL sources (limited by number of hardware channels)
		*/
		
		double volumeComponent = 0;
		{
			if (audioInstance->threeD) {
				double xDistance = audioInstance->positionX - listenerPositionX;
				double yDistance = audioInstance->positionY - listenerPositionY;
				double distance = std::sqrt(xDistance * xDistance + yDistance * yDistance);

				if (distance != 0) {
					// Basically the linear attenuation function (with rolloff = 1)
					volumeComponent += 1 - (distance - audioInstance->size) / (audioInstance->range - audioInstance->size);
				}

				// Clamp volume
				volumeComponent = volumeComponent;
				if (volumeComponent < 0) volumeComponent = 0;
				if (volumeComponent > 1) volumeComponent = 1;
			}
			else {
				volumeComponent = 1.0;
			}
			
			/*	Note on use of sqrt:
			*	The use of sqrt here shoulld >>not<< be a bottle neck, as this heuristic
			*   calculation should be done a reasonably low number of times per frame.
			*	A seemingly simple solution, at first glance, might be to raise both the
			*	numerator and denominator to the power of 2, but this will not provide
			*	the same result.
			*/
			volumeComponent *= audioInstance->volume;
		}

		// Audio priority
		double priorityComponent = 0;
		{
			priorityComponent += audioInstance->priority;
			priorityComponent += audioInstance->asset->getPriority();
		}

		// Instance play count (how many instance of asset is playing)
		double instanceCountComponent = 0;
		{
			/*
			 * The order that the heuristics are calculated for each
			 * AudioInstance influences this, as the first AudioInstance
			 * of a given asset will get the highest priority
			 */
			assetInstanceCount[audioInstance->asset]++;
			instanceCountComponent = assetInstanceCount[audioInstance->asset];
		}

		double heuristic = (volumeComponent * priorityComponent) / instanceCountComponent;
		audioInstance->heuristic = heuristic;
		return heuristic;
	}


	void AudioSystem::activateInstance(AudioInstance* instance) {
		if (ALData::freeSources.empty())
			throw new InvalidStateException("No OpenAL sources are available when activating AudioInstance");

		ALuint* sourceId = ALData::freeSources.top();
		ALData::freeSources.pop();
	
		instance->activate(sourceId);
	}


	void AudioSystem::deactivateInstance(AudioInstance* instance) {
		void* nativeObject = instance->deactivate();
		ALuint* sourceId = static_cast<ALuint*>(nativeObject);
		ALData::freeSources.push(sourceId);
	}


	void AudioSystem::setListenerPosition(double positionX, double positionY) {
		listenerPositionX = positionX;
		listenerPositionY = positionY;
 		alListener3f(AL_POSITION, (ALfloat)listenerPositionX, (ALfloat)listenerPositionY, (ALfloat)-listenerDepth);
		ALUtility::checkErrors();
	}


	void AudioSystem::setListenerVelocity(double velocityX, double velocityY) {
		listenerVelocityX = velocityX;
		listenerVelocityY = velocityY;
		alListener3f(AL_VELOCITY, (ALfloat)listenerVelocityX, (ALfloat)listenerVelocityY, 0);
		ALUtility::checkErrors();
	}

	
	void AudioSystem::setListenerDepth(double depth){
		listenerDepth = depth;
		alListener3f(AL_POSITION, (ALfloat)listenerPositionX, (ALfloat)listenerPositionY, (ALfloat)-listenerDepth);
		ALUtility::checkErrors();
	}


	double AudioSystem::getListenerDepth(){
		return listenerDepth;
	}


	void AudioSystem::setListenerRotation(double rotation){
		rotation = fmod(rotation, 360);
		if( rotation < 0 ) rotation += 360;
		listenerRotation = rotation;
		
		// Adjust rotation by adding 90 degrees, to make 0 degrees
		// point upwards, and convert it to radians in range -PI to PI
		double radianRotation;
		radianRotation = listenerRotation + 90;
		if( radianRotation >= 360 ) radianRotation - 360; // Reclamp to 0-360
		if( radianRotation > 180 ) radianRotation - 360; // Adjust to -180 to 108
		radianRotation *= 0.01745329251;

		// Get 2D direction vector
		double directionX = cos(radianRotation);
		double directionY = sin(radianRotation);

		// With an "at" vector looking "towards the screen" (along
		// the negative z-axis), we can use the up vector to rotate,
		// around the direction, effectively rotation our 2D listener
		ALfloat orientation[] = {
							  0,				   0, -1.0f, // At vector
			(ALfloat)directionX, (ALfloat)directionY,     0  // Up vector
		};
		alListenerfv(AL_ORIENTATION, orientation);
		ALUtility::checkErrors();
	}

	
	double AudioSystem::getListenerRotation(){
		return listenerRotation;
	}

}



