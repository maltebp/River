
#include "AudioSystem.h"

#include <vector>
#include <stack>
#include <algorithm>

#include "River.h"
#include "ALUtility.h"

namespace River {


	struct ALData {
		static inline const int NUM_SOURCES = 16;
		static inline std::unordered_map<AudioInstance*, ALuint> instanceSourceMap;
		static inline std::stack<ALuint> freeSources;
		static inline std::vector<ALuint> allSources;
	};


	void AudioInstance::activate() {
		if (active) return;

		if (ALData::freeSources.empty())
			throw new InvalidStateException("No OpenAL sources are available when activating AudioInstance");

		ALuint sourceId = ALData::freeSources.top();
		ALData::freeSources.pop();
		ALData::instanceSourceMap[this] = sourceId;

		alSourcei(sourceId, AL_BUFFER, *static_cast<ALuint*>(asset->getData()));
		// Setting buffer position (in seconds)
		alSourcef(sourceId, AL_SEC_OFFSET, currentTime);

		// Set source attributes
		alSourcef(sourceId, AL_GAIN, (float)volume);
		alSourcef(sourceId , AL_PITCH, 1);
		alSourcei(sourceId, AL_LOOPING, looping);
		alSource3f(sourceId, AL_POSITION, positionX, positionY, 0);
		alSource3f(sourceId, AL_POSITION, velocityX, velocityY, 0);

		alSourcePlay(sourceId);
		
		ALUtility::checkErrors();

		active = true;
	}


	void AudioInstance::deactivate() {
		auto it = ALData::instanceSourceMap.find(this);
		if (it == ALData::instanceSourceMap.end()) return;

		ALuint sourceId = it->second;
		ALData::instanceSourceMap.erase(it);

		alSourceStop(sourceId);
		ALUtility::checkErrors();		

		ALData::freeSources.push(sourceId);

		active = false;
	}


	
	void AudioSystem::initialize() {
		if (initialized) throw new InvalidStateException("Audio system has already been initialized");
		initialized = true;

		// Open default device
		ALCdevice* device = alcOpenDevice(nullptr);
		if (device == nullptr) throw new AudioException("Could not open preffered audio device");

		// Not sure if we should check if eax2.0 exists here
		// The OpenAL programming guide does, but the return
		// result is never used
		
		// Open context
		ALCcontext* context = alcCreateContext(device, nullptr);
		alcMakeContextCurrent(context);
		ALUtility::checkContextError(device);
		
		// Generate sources
		for (int i = 0; i < ALData::NUM_SOURCES; i++) {
			ALuint sourceId;
			alGenSources(1, &sourceId);
			ALUtility::checkErrors();
			ALData::allSources.push_back(sourceId);
			ALData::freeSources.push(sourceId);
		}
	}


	void AudioSystem::update(double time) {
		if (!initialized) throw new InvalidStateException("Audio system has not been initialized");

		// Reset number of audio instances for assets
		// (see heuristics comment)
		for (auto& it : assetInstanceCount)
			it.second = 0;
		
		std::vector<AudioInstance*> finishedInstances;
		for (auto instance : audioInstances) {
			// Check instance time
			instance->currentTime += time;
			if (instance->currentTime > instance->length) {
				if (!instance->looping) {
					finishedInstances.push_back(instance);
					continue;
				}
				instance->currentTime = std::fmod(instance->currentTime, instance->length);
			}

			// Calculate heuristic
			calculateHeuristic(instance);		
		}

		for (auto instance : finishedInstances) {
			auto instanceIterator = std::find(audioInstances.begin(), audioInstances.end(), instance);
			audioInstances.erase(instanceIterator);
			// TODO: Remove from sources and fire on finished callback
		}

		// Sort instance with heuristic
		std::vector<AudioInstance*> sortedInstances(audioInstances);
		std::sort(sortedInstances.begin(), sortedInstances.end(), [](AudioInstance* a1, AudioInstance* a2) {
			return a1 - a2;
		});

		// Deactivate instances that should not be active, but are
		if (sortedInstances.size() > ALData::NUM_SOURCES) {
			for (auto iterator = sortedInstances.begin() + ALData::NUM_SOURCES; iterator != sortedInstances.end(); iterator++) {
				AudioInstance* instance = *iterator;
				if (instance->active)
					instance->deactivate();
			}
		}


		for (int i = 0; i < ALData::NUM_SOURCES && i < sortedInstances.size(); i++) {
			sortedInstances[i]->activate();
		}
		

	}


	double AudioSystem::calculateHeuristic(AudioInstance* audioInstance) {
		/*	An AudioInstance's heuristic is used to determine whether or not
		*   it should be played, when there are more AudioInstances present
		*   than there are OpenAL sounds (limited by number of hardware channels)
		*/
		
		double volumeComponent = 0;
		{
			double xDistance = audioInstance->positionX - listenerPositionX;
			double yDistance = audioInstance->positionY - listenerPositionY;
			volumeComponent += referenceDistance / std::sqrt(xDistance * xDistance + yDistance * yDistance);
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

		double heuristic = volumeComponent * priorityComponent / instanceCountComponent;
		audioInstance->heuristic = heuristic;
		return heuristic;
	}


	void AudioSystem::playAudio(AudioAsset* asset) {


		if (!asset->isStream()) {
			AudioInstance* instance = new AudioInstance();
			instance->asset = asset;
			instance->length = asset->getLength();
			instance->priority = 100;
			audioInstances.push_back(instance);
		}
	}


	void AudioSystem::setMasterVolume(double volume) {
		volume = volume > 1.0 ? 1.0 : volume;
		volume = volume < 0   ?   0	: volume;
		masterVolume = volume;
	}


	double AudioSystem::getMasterVolume() {
		return masterVolume;
	}


	void AudioSystem::setReferenceDistance(double distance) {
		if (distance <= 0)
			throw new InvalidArgumentException("Reference distance must be larger than 0");
		referenceDistance = distance;
		
		for(auto sourceId : ALData::allSources ) {
			alSourcef(sourceId, AL_REFERENCE_DISTANCE, distance);
			ALUtility::checkErrors();
		}
	}


	double AudioSystem::getReferenceDistance() {
		return referenceDistance;
	}


	void AudioSystem::setListenerPosition(double positionX, double positionY) {
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



