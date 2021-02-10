#include "AudioSystem.h"

#include <vector>
#include <stack>
#include <algorithm>

#include "River.h"
#include "ALData.h"
#include "ALUtility.h"


namespace River {


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
			ALData::allSources.push_back(sourceId);
			ALData::freeSources.push(sourceId);
		}
	}


	void AudioSystem::playAudio(AudioInstance* audio) {
		if (audio->playing) return;
		audio->playing = true;
		playingInstances.push_back(audio);
	}


	void AudioSystem::stopAudio(AudioInstance* audio) {
		if (!audio->playing) return;
		playingInstances.erase(std::find(playingInstances.begin(), playingInstances.end(), audio));

		if (audio->active) {
			// Stop and release AL source
			ALuint sourceId = ALData::instanceSourceMap.at(audio);
			alSourceStop(sourceId);
			ALData::instanceSourceMap.erase(audio);
			ALData::freeSources.push(sourceId);
		}

		audio->playing = false;
		audio->active = false;
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

			if( !instance->active ) {
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
				// Use AL to check time
				ALuint sourceId = ALData::instanceSourceMap.at(instance);
				ALenum state;
				alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
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
			if( instance->active )
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
				if (instance->active)
					deactivateInstance(instance);
				continue;
			}

			if (!instance->active)
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
					volumeComponent += 1 - (distance - audioInstance->size) / (audioInstance->range - audioInstance->size);

				}

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

		ALuint sourceId = ALData::freeSources.top();
		ALData::freeSources.pop();
		ALData::instanceSourceMap[instance] = sourceId;

		alSourcei(sourceId, AL_BUFFER, *static_cast<ALuint*>(instance->asset->getData()));

		alSourcef(sourceId, AL_SEC_OFFSET, instance->currentTime);
		std::cout << "ACtivation time: " << instance->currentTime << std::endl;

		alSourcef(sourceId, AL_REFERENCE_DISTANCE, instance->size);
		alSourcef(sourceId, AL_MAX_DISTANCE, instance->range);

		alSourcef(sourceId, AL_GAIN, (ALfloat)instance->volume);
		alSourcef(sourceId, AL_PITCH, (ALfloat)instance->speed);
		alSourcei(sourceId, AL_LOOPING, (ALfloat)instance->looping);
		

		// Set position (in accordance with 3d flag)
		if (instance->threeD) {
			alSourcei(sourceId, AL_SOURCE_RELATIVE, 0);
			alSource3f(sourceId, AL_POSITION, (ALfloat)instance->positionX, (ALfloat)instance->positionY, 0);
			alSource3f(sourceId, AL_VELOCITY, (ALfloat)instance->velocityX, (ALfloat)instance->velocityY, 0);
		}
		else {
			alSourcei(sourceId, AL_SOURCE_RELATIVE, 1);
			alSource3f(sourceId, AL_POSITION, 0, 0, 0);
			alSource3f(sourceId, AL_VELOCITY, 0, 0, 0);
		}

		alSourcePlay(sourceId);

		ALUtility::checkErrors();

		instance->active = true;
	}


	void AudioSystem::deactivateInstance(AudioInstance* instance) {
		auto it = ALData::instanceSourceMap.find(instance);
		if (it == ALData::instanceSourceMap.end()) return;

		ALuint sourceId = it->second;
		ALData::instanceSourceMap.erase(it);

		alSourceStop(sourceId);
		ALUtility::checkErrors();

		ALData::freeSources.push(sourceId);

		instance->active = false;
	}


	void AudioSystem::setMasterVolume(double volume) {
		masterVolume = volume;
		masterVolume = masterVolume > 1.0 ? 1.0 : masterVolume;
		masterVolume = masterVolume < 0 ? 0 : masterVolume;
		alListenerf(AL_GAIN, masterVolume);
		ALUtility::checkErrors();
	}


	void AudioSystem::adjustMasterVolume(double volume) {
		masterVolume += volume;
		masterVolume = masterVolume > 1.0 ? 1.0 : masterVolume;
		masterVolume = masterVolume < 0 ? 0 : masterVolume;
		alListenerf(AL_GAIN, masterVolume);
		ALUtility::checkErrors();
	}


	double AudioSystem::getMasterVolume() {
		return masterVolume;
	}


	void AudioSystem::setListenerPosition(double positionX, double positionY) {
		// Throws error when moving
		listenerPositionX = positionX;
		listenerPositionY = positionY;
 		alListener3f(AL_POSITION, positionX, positionY, 0.0f);
		ALUtility::checkErrors();
	}


	void AudioSystem::setListenerVelocity(double velocityX, double velocityY) {
		listenerVelocityX = velocityX;
		listenerVelocityY = velocityY;
		alListener3f(AL_VELOCITY, velocityX, velocityY, 0.0f);
		ALUtility::checkErrors();
	}


}



