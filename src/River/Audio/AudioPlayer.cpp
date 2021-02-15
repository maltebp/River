#include "AudioPlayer.h"

#include <iostream>
#include <functional>
#include <algorithm>
#include <set>

#include "AL.h"
#include "AudioListener.h"

namespace River {


	class Globals {
	public:

		// ALL audio instances that are currently playing (in the order they were triggered)
		static inline std::set<AudioPlayer*> playingPlayers;

		// Mapping of how many audio instances uses the a given AudioAsset
		static inline std::unordered_map<AudioAsset*, unsigned int> assetPlayerCount;
	

		// These repeatedly cleared and used in the update function
		static inline std::vector<AudioPlayer*> finishedPlayers;
		static inline std::vector<AudioPlayer*> sortedPlayers;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	AudioPlayer::AudioPlayer() {
	}


	AudioPlayer::~AudioPlayer() {
		stop();
	}


	void AudioPlayer::play(AudioAsset* asset) {
		this->asset = asset;
		setTime(0);
		paused = false;

		if( !playing ) {
			Globals::playingPlayers.insert(this);
			playing = true;
		}
	}


	bool AudioPlayer::isPlaying() {
		return playing;
	}


	void AudioPlayer::stop() {
		if( playing ) {
			// Stop from playing
			deactivate();
			Globals::playingPlayers.erase(Globals::playingPlayers.find(this));
			playing = false;
		}

		// Clear the paused state
		setTime(0);
		paused = false;
	}


	void AudioPlayer::pause() {
		if( !playing ) return;
	
		// Stop from playing
		deactivate();
		Globals::playingPlayers.erase(Globals::playingPlayers.find(this));
		
		playing = false;
		paused = true;
	}


	void AudioPlayer::unpause() {
		if( !paused ) return;

		Globals::playingPlayers.insert(this);
		playing = true;
		paused = false;
	}


	bool AudioPlayer::isPaused() {
		return paused;
	}


	void AudioPlayer::setPriority(unsigned int priority) {
		this->priority = priority;
	}


	unsigned int AudioPlayer::getPriority() {
		return priority;
	}


	void AudioPlayer::setLooping(bool toggle) {
		if( nativeObject != nullptr && spatial ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSourcei(sourceId, AL_LOOPING, toggle);
		}
		looping = toggle;
	}


	bool AudioPlayer::isLooping() {
		return true;
	}


	void AudioPlayer::setSpeed(double speed) {
		if( speed <= 0 )
			throw new InvalidArgumentException("AudioPlayer speed must be above 0");
		if( nativeObject != nullptr && spatial ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSourcef(sourceId, AL_PITCH, static_cast<ALfloat>(speed));
		}
		this->speed = speed;
	}


	double AudioPlayer::getSpeed(double speed) {
		return speed;
	}


	void AudioPlayer::setVolume(double volume) {
		if( volume < 0 )
			throw new InvalidArgumentException("Audio volume may not be less than 0");
		this->volume = volume;

		if( nativeObject != nullptr && spatial ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSourcef(sourceId, AL_GAIN, static_cast<ALfloat>(this->volume));
		}
	}


	double AudioPlayer::getVolume() {
		return volume;
	}


	void AudioPlayer::setSize(double size) {
		if( size < 0 )
			throw new InvalidArgumentException("Audio size must be larger than or equal to 0");
		if( size >= range )
			throw new InvalidArgumentException("Audio size must be less than its range");

		this->size = size;

		if( nativeObject != nullptr && spatial ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSourcef(sourceId, AL_REFERENCE_DISTANCE, (ALfloat)size);
			AL::checkErrors();
		}
	}


	double AudioPlayer::getSize() {
		return size;
	}


	void AudioPlayer::setRange(double range) {
		if( this->range <= 0 )
			throw new InvalidArgumentException("Audio range must be larger than 0");
		if( this->range <= size )
			throw new InvalidArgumentException("Audio range must be larger than its size");
		this->range = range;

		if( nativeObject != nullptr && spatial ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSourcef(sourceId, AL_MAX_DISTANCE, (ALfloat)range);
			AL::checkErrors();
		}
	}


	double AudioPlayer::getRange() {
		return range;
	}


	void AudioPlayer::setPosition(double positionX, double positionY) {
		this->positionX = positionX;
		this->positionY = positionY;

		if( nativeObject != nullptr && spatial ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSource3f(sourceId, AL_POSITION, (ALfloat)positionX, (ALfloat)positionY, (ALfloat)-depth);
			AL::checkErrors();
		}
	}


	double AudioPlayer::getPositionX() {
		return positionX;
	}


	double AudioPlayer::getPositionY() {
		return positionY;
	}


	void AudioPlayer::setVelocity(double velocityX, double velocityY) {
		if( nativeObject != nullptr && spatial ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSource3f(sourceId, AL_VELOCITY, (ALfloat)velocityX, (ALfloat)velocityY, 0);
			AL::checkErrors();
		}
		this->velocityX = velocityX;
		this->velocityY = velocityY;
	}


	void AudioPlayer::setDepth(double depth){
		if( depth < 0 )
			throw new InvalidArgumentException("Depth must not be less than 0");
		this->depth = depth;
			
		if( nativeObject != nullptr && spatial ){
			ALuint sourceId = *static_cast<ALuint*>(nativeObject);
			alSource3f(sourceId, AL_POSITION, (ALfloat)positionX, (ALfloat)positionY, (ALfloat)-this->depth);
			AL::checkErrors();
		}
	}


	double AudioPlayer::getDepth(double depth){
		return depth;
	}


	void AudioPlayer::setSpatial(bool toggle) {
		if( nativeObject != nullptr && toggle != spatial ) {
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
		spatial = toggle;
	}


	bool AudioPlayer::isSpatial() {
		return spatial;
	}


	void AudioPlayer::setTime(double time) {
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


	double AudioPlayer::getTime() {
		return currentTime;
	}	


	void AudioPlayer::onFinish(std::function<void(AudioPlayer*)> callback) {
		onFinishCallback = callback;
	}


	AudioAsset* AudioPlayer::getAsset() {
		return asset;
	}


	void AudioPlayer::activate(){
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
		if( spatial ){
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

	
	void AudioPlayer::deactivate(){
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


	bool AudioPlayer::isActive() {
		return nativeObject != nullptr;
	}


	double AudioPlayer::calculateHeuristic() {
		/*	An AudioPlayer's heuristic is used to determine whether or not
		*   it should be played, when there are more AudioInstances present
		*   than there are OpenAL sources (limited by number of hardware channels)
		*/

		double volumeComponent = 0;
		{
			if( spatial ) {
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
			* AudioPlayer influences this, as the first AudioPlayer
			* of a given asset will get the highest priority
			*/
			Globals::assetPlayerCount[asset]++;
			instanceCountComponent = Globals::assetPlayerCount[asset];
		}

		heuristic = (volumeComponent * priorityComponent) / instanceCountComponent;
		return heuristic;
	}


	void AudioPlayer::updatePlayers(double time) {
		if( !AL::isInitialized() ) throw new InvalidStateException("Audio system has not been initialized");

		if( Globals::playingPlayers.size() == 0 ) return;

		// Reset number of audio instances for assets
		// (used for heuristic calculation)
		for( auto& it : Globals::assetPlayerCount )
			it.second = 0;

		// Update time and heuristics
		std::vector<AudioPlayer*>& finishedPlayers = Globals::finishedPlayers;
		finishedPlayers.clear();
		for( auto instance : Globals::playingPlayers ) {
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

				ALint state;
				alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
				if( state == AL_STOPPED )
					finished = true;	
			}

			// Check if finished
			if( finished ) {
				finishedPlayers.push_back(instance);
				continue;
			}

			// Calculate heuristic
			instance->calculateHeuristic();
		}

		// Remove finished instances
		for( auto instance : finishedPlayers ) {
			instance->stop();
			if( instance->onFinishCallback != nullptr )
				instance->onFinishCallback(instance);
		}

		// Sort instance with heuristic
		// Sorting in new list, as a audioInstances, is in the order
		// they have been "played"
		std::vector<AudioPlayer*>& sortedPlayers = Globals::sortedPlayers;
		sortedPlayers.clear();
		sortedPlayers.resize(Globals::playingPlayers.size());
		std::copy(Globals::playingPlayers.begin(), Globals::playingPlayers.end(), Globals::sortedPlayers.begin());
		std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](AudioPlayer* a1, AudioPlayer* a2) {
			return a1->heuristic > a2->heuristic;
			});

		// Stop all instances that are playing but shouldn't be
		for( int i = AL::NUM_SOURCES; i < sortedPlayers.size() ; i++ ) {
			AudioPlayer* instance = sortedPlayers[i];
			if( instance->isActive() ) instance->deactivate();
		}

		// Start all instances that aren't playing but should be
		for( int i = 0; i < sortedPlayers.size() && i < AL::NUM_SOURCES; i++ ) {
			AudioPlayer* instance = sortedPlayers[i];
			if( !instance->isActive() ) instance->activate();
		}
	}

}