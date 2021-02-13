#include "AudioInstance.h"

#include <iostream>
#include <functional>
#include <algorithm>

#include "AL.h"
#include "AudioListener.h"

namespace River {


	class Globals {
	public:

		// ALL audio instances that are currently playing
		static inline std::vector<AudioInstance*> playingInstances; // TODO: Convert to unordered set

		// Mapping of how many audio instances uses the a given AudioAsset
		static inline std::unordered_map<AudioAsset*, unsigned int> assetInstanceCount;
	
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


	AudioInstance::AudioInstance(AudioAsset* asset) {
		this->asset = asset;
	}


	AudioInstance::~AudioInstance() {
		stop();
	}


	void AudioInstance::play() {
		setTime(0);
		paused = false;

		if( !playing ) {
			Globals::playingInstances.push_back(this);
			playing = true;
		}
	}


	bool AudioInstance::isPlaying() {
		return playing;
	}


	void AudioInstance::stop() {
		if( playing ) {
			// Stop from playing
			deactivate();
			auto instanceIterator = std::find(Globals::playingInstances.begin(), Globals::playingInstances.end(), this);
			Globals::playingInstances.erase(instanceIterator);
			playing = false;
		}

		// Clear the paused state
		setTime(0);
		paused = false;
	}


	void AudioInstance::pause() {
		if( !playing ) return;
	
		// Stop from playing
		deactivate();
		auto instanceIterator = std::find(Globals::playingInstances.begin(), Globals::playingInstances.end(), this);
		Globals::playingInstances.erase(instanceIterator);
		
		playing = false;
		paused = true;
	}


	void AudioInstance::unpause() {
		if( !paused ) return;

		Globals::playingInstances.push_back(this);
		playing = true;
		paused = false;
	}


	bool AudioInstance::isPaused() {
		return paused;
	}


	void AudioInstance::setPriority(unsigned int priority) {
		this->priority = priority;
	}


	unsigned int AudioInstance::getPriority() {
		return priority;
	}


	void AudioInstance::setLooping(bool toggle) {
		if( nativeObject != nullptr && threeD ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSourcei(sourceId, AL_LOOPING, toggle);
		}
		looping = toggle;
	}


	bool AudioInstance::isLooping() {
		return true;
	}


	void AudioInstance::setSpeed(double speed) {
		speed = speed < 0.0 ? 0.0 : speed;
		if( nativeObject != nullptr && threeD ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSourcef(sourceId, AL_PITCH, static_cast<ALfloat>(speed));
		}
		this->speed = speed;
	}


	double AudioInstance::getSpeed(double speed) {
		return speed;
	}


	void AudioInstance::setVolume(double volume) {
		if( volume < 0 )
			throw new InvalidArgumentException("Audio volume may not be less than 0");
		this->volume = volume;

		if( nativeObject != nullptr && threeD ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSourcef(sourceId, AL_GAIN, static_cast<ALfloat>(this->volume));
		}
	}


	double AudioInstance::getVolume() {
		return volume;
	}


	void AudioInstance::setSize(double size) {
		if( size < 0 )
			throw new InvalidArgumentException("Audio size must be larger than or equal to 0");
		if( size >= range )
			throw new InvalidArgumentException("Audio size must be less than its range");

		this->size = size;

		if( nativeObject != nullptr && threeD ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSourcef(sourceId, AL_REFERENCE_DISTANCE, (ALfloat)size);
			AL::checkErrors();
		}
	}


	double AudioInstance::getSize() {
		return size;
	}


	void AudioInstance::setRange(double range) {
		if( this->range <= 0 )
			throw new InvalidArgumentException("Audio range must be larger than 0");
		if( this->range <= size )
			throw new InvalidArgumentException("Audio range must be larger than its size");
		this->range = range;

		if( nativeObject != nullptr && threeD ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSourcef(sourceId, AL_MAX_DISTANCE, (ALfloat)range);
			AL::checkErrors();
		}
	}


	double AudioInstance::getRange() {
		return range;
	}


	void AudioInstance::setPosition(double positionX, double positionY) {
		this->positionX = positionX;
		this->positionY = positionY;

		if( nativeObject != nullptr && threeD ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSource3f(sourceId, AL_POSITION, (ALfloat)positionX, (ALfloat)positionY, (ALfloat)-depth);
			AL::checkErrors();
		}
	}


	double AudioInstance::getPositionX() {
		return positionX;
	}


	double AudioInstance::getPositionY() {
		return positionY;
	}


	void AudioInstance::setVelocity(double velocityX, double velocityY) {
		if( nativeObject != nullptr && threeD ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSource3f(sourceId, AL_VELOCITY, (ALfloat)velocityX, (ALfloat)velocityY, 0);
			AL::checkErrors();
		}
		this->velocityX = velocityX;
		this->velocityY = velocityY;
	}


	void AudioInstance::setDepth(double depth){
		this->depth = depth;

		if( nativeObject != nullptr && threeD ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSource3f(sourceId, AL_POSITION, (ALfloat)positionX, (ALfloat)positionY, (ALfloat)-this->depth);
			AL::checkErrors();
		}
	}


	double AudioInstance::getDepth(double depth){
		return depth;
	}


	void AudioInstance::set3D(bool toggle) {
		if( nativeObject != nullptr && toggle != threeD ) {
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			if( toggle ) {
				alSourcei(sourceId, AL_SOURCE_RELATIVE, 0);
				alSource3f(sourceId, AL_POSITION, (ALfloat)positionX, (ALfloat)positionY, (ALfloat)-depth);
				alSource3f(sourceId, AL_VELOCITY, (ALfloat)velocityX, (ALfloat)velocityY, 0);
			}
			else {
				alSourcei(sourceId, AL_SOURCE_RELATIVE, 1);
				alSource3f(sourceId, AL_POSITION, 0, 0, 0);
				alSource3f(sourceId, AL_VELOCITY, 0, 0, 0);
			}
			AL::checkErrors();
		}
		threeD = toggle;
	}


	bool AudioInstance::is3D() {
		return threeD;
	}


	void AudioInstance::setTime(double time) {
		if( time < 0 )
			throw InvalidArgumentException("Time must not be less than 0");

		// Adjust according to length of audio clip
		if( time > asset->getLength() ) {
			if( looping )
				time = std::fmod(time, asset->getLength());
			else
				time = asset->getLength();
		}

		currentTime = time;

		if( isActive() ) {
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSourcef(sourceId, AL_SEC_OFFSET, (ALfloat)time);
			AL::checkErrors();
		}
	}


	double AudioInstance::getTime() {
		return currentTime;
	}	


	void AudioInstance::onFinish(std::function<void(AudioInstance*)> callback) {
		onFinishCallback = callback;
	}


	AudioAsset* AudioInstance::getAsset() {
		return asset;
	}


	void AudioInstance::activate(){
		if( nativeObject != nullptr )
			throw new InvalidStateException("AudioInstance is already active");

		ALuint* sourceIdPtr = AL::getSource();
		nativeObject = sourceIdPtr;
		ALuint sourceId = *sourceIdPtr;

		alSourcei(sourceId, AL_BUFFER, *static_cast<ALuint*>(asset->getData()));

		alSourcef(sourceId, AL_SEC_OFFSET, (ALfloat)currentTime);

		alSourcef(sourceId, AL_REFERENCE_DISTANCE, (ALfloat)size);
		alSourcef(sourceId, AL_MAX_DISTANCE, (ALfloat)range);

		alSourcef(sourceId, AL_GAIN, (ALfloat)volume);
		alSourcef(sourceId, AL_PITCH, (ALfloat)speed);
		alSourcei(sourceId, AL_LOOPING, (ALint)looping);

		// Set position (in accordance with 3d flag)
		if( threeD ){
			alSourcei(sourceId, AL_SOURCE_RELATIVE, 0);
			alSource3f(sourceId, AL_POSITION, (ALfloat)positionX, (ALfloat)positionY, (ALfloat)-depth);
			alSource3f(sourceId, AL_VELOCITY, (ALfloat)velocityX, (ALfloat)velocityY, 0);
		}
		else{
			alSourcei(sourceId, AL_SOURCE_RELATIVE, 1);
			alSource3f(sourceId, AL_POSITION, 0, 0, 0);
			alSource3f(sourceId, AL_VELOCITY, 0, 0, 0);
		}

		alSourcePlay(sourceId);
		AL::checkErrors();

		this->nativeObject = nativeObject;
	}

	
	void AudioInstance::deactivate(){
		if( !isActive() )
			throw new InvalidStateException("Audio instance is not active");
		
		ALuint* sourceId = static_cast<ALuint*>(nativeObject);
		alSourceStop(*sourceId);
		AL::checkErrors();

		alSourcei(*sourceId, AL_BUFFER, NULL);
		AL::checkErrors();

		AL::releaseSource(sourceId);
	
		nativeObject = nullptr;
	}


	bool AudioInstance::isActive() {
		return nativeObject != nullptr;
	}


	double AudioInstance::calculateHeuristic() {
		/*	An AudioInstance's heuristic is used to determine whether or not
		*   it should be played, when there are more AudioInstances present
		*   than there are OpenAL sources (limited by number of hardware channels)
		*/

		double volumeComponent = 0;
		{
			if( threeD ) {
				double xDistance = positionX - AudioListener::getPositionX();
				double yDistance = positionY - AudioListener::getPositionY();
				double distance = std::sqrt(xDistance * xDistance + yDistance * yDistance);

				if( distance != 0 ) {
					// Basically the linear attenuation function (with rolloff = 1)
					volumeComponent += 1 - (distance - size) / (range - size);
				}

				// Clamp volume
				if( volumeComponent < 0 ) volumeComponent = 0;
				if( volumeComponent > 1 ) volumeComponent = 1;
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
			volumeComponent *= volume;
		}

		// Audio priority
		double priorityComponent = 0;
		{
			priorityComponent += priority;
			priorityComponent += asset->getPriority();
		}

		// Instance play count (how many instance of asset is playing)
		double instanceCountComponent = 0;
		{
			/*
				* The order that the heuristics are calculated for each
				* AudioInstance influences this, as the first AudioInstance
				* of a given asset will get the highest priority
				*/
			Globals::assetInstanceCount[asset]++;
			instanceCountComponent = Globals::assetInstanceCount[asset];
		}

		heuristic = (volumeComponent * priorityComponent) / instanceCountComponent;
		return heuristic;
	}


	void AudioInstance::updateInstances(double time) {
		if( !AL::isInitialized() ) throw new InvalidStateException("Audio system has not been initialized");

		if( Globals::playingInstances.size() == 0 ) return;

		// Reset number of audio instances for assets
		// (used for heuristic calculation)
		for( auto& it : Globals::assetInstanceCount )
			it.second = 0;

		// Update time and heuristics
		std::vector<AudioInstance*> finishedInstances;
		for( auto instance : Globals::playingInstances ) {
			bool finished = false;

			if( !instance->isActive() ) {
				// Not active: Manually update time
				instance->currentTime += time * instance->speed;
				if( instance->currentTime > instance->asset->getLength() ) {
					if( !instance->looping )
						finished = true;
					else
						instance->currentTime = std::fmod(instance->currentTime, instance->asset->getLength());
				}
			}
			else {
				// Use AL to check if finished
				ALuint sourceId = *static_cast<ALuint*>(instance->nativeObject);
				
				ALfloat time;
				alGetSourcef(sourceId, AL_SEC_OFFSET, &time);
				AL::checkErrors();
				instance->currentTime = time;
				if( !instance->looping && time >= instance->asset->getLength() )
					finished = true;	
			}

			// Check if finished
			if( finished ) {
				finishedInstances.push_back(instance);
				continue;
			}

			// Calculate heuristic
			instance->calculateHeuristic();
		}

		// Remove finished instances
		for( auto instance : finishedInstances ) {
			instance->stop();
			if( instance->onFinishCallback != nullptr )
				instance->onFinishCallback(instance);
		}

		// Sort instance with heuristic
		// Sorting in new list, as a audioInstances, is in the order
		// they have been "played"
		std::vector<AudioInstance*> sortedInstances(Globals::playingInstances);
		std::sort(sortedInstances.begin(), sortedInstances.end(), [](AudioInstance* a1, AudioInstance* a2) {
			return a1->heuristic > a2->heuristic;
			});

		// Stop all instances that are playing but shouldn't be
		for( int i = AL::NUM_SOURCES; i < sortedInstances.size() ; i++ ) {
			AudioInstance* instance = sortedInstances[i];
			if( instance->isActive() ) instance->deactivate();
		}

		// Start all instances that aren't playing but should be
		for( int i = 0; i < sortedInstances.size() && i < AL::NUM_SOURCES; i++ ) {
			AudioInstance* instance = sortedInstances[i];
			if( !instance->isActive() ) instance->activate();
		}
	}

}