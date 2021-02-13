#include "AudioInstance.h"

#include <iostream>
#include <functional>

#include "ALData.h"
#include "ALUtility.h"
#include "AudioSystem.h"

namespace River {


	AudioInstance::AudioInstance(AudioAsset* asset) {
		this->asset = asset;
	}


	AudioInstance::~AudioInstance() {
		if( playing )
			stop();
	}


	void AudioInstance::play() {
		AudioSystem::playAudio(this);
	}


	bool AudioInstance::isPlaying() {
		return playing;
	}


	void AudioInstance::setPriority(unsigned int priority) {
		this->priority = priority;
	}


	unsigned int AudioInstance::getPriority() {
		return priority;
	}


	void AudioInstance::setLooping(bool toggle) {
		if( active ) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSourcei(sourceId, AL_LOOPING, toggle);
		}
		looping = toggle;
	}


	bool AudioInstance::isLooping() {
		return true;
	}


	void AudioInstance::setSpeed(double speed) {
		speed = speed < 0.0 ? 0.0 : speed;
		if( active ) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
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

		if( active ) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSourcef(sourceId, AL_GAIN, static_cast<ALfloat>(this->volume));
		}
	}


	void AudioInstance::setSize(double size) {
		if( size < 0 )
			throw new InvalidArgumentException("Audio size must be larger than or equal to 0");
		if( size >= range )
			throw new InvalidArgumentException("Audio size must be less than its range");

		this->size = size;

		if( active ) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSourcef(sourceId, AL_REFERENCE_DISTANCE, (ALfloat)size);
			ALUtility::checkErrors();
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

		if( active ) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSourcef(sourceId, AL_MAX_DISTANCE, (ALfloat)range);
			ALUtility::checkErrors();
		}
	}


	double AudioInstance::getRange() {
		return range;
	}


	void AudioInstance::setPosition(double positionX, double positionY) {
		this->positionX = positionX;
		this->positionY = positionY;

		if( active && threeD ) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSource3f(sourceId, AL_POSITION, (ALfloat)positionX, (ALfloat)positionY, (ALfloat)-depth);
			ALUtility::checkErrors();
		}
	}


	void AudioInstance::setVelocity(double velocityX, double velocityY) {
		if( active && threeD ) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSource3f(sourceId, AL_VELOCITY, (ALfloat)velocityX, (ALfloat)velocityY, 0);
			ALUtility::checkErrors();
		}
		this->velocityX = velocityX;
		this->velocityY = velocityY;
	}


	void AudioInstance::setDepth(double depth){
		this->depth = depth;

		if( active && threeD ){
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSource3f(sourceId, AL_POSITION, (ALfloat)positionX, (ALfloat)positionY, (ALfloat)-this->depth);
			ALUtility::checkErrors();
		}
	}


	double AudioInstance::getDepth(double depth){
		return depth;
	}


	void AudioInstance::set3D(bool toggle) {
		if( active && toggle != threeD ) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
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
			ALUtility::checkErrors();
		}
		threeD = toggle;
		dirty = true;
	}


	bool AudioInstance::is3D() {
		return threeD;
	}


	void AudioInstance::setTime(double time) {
		time = time < 0 ? 0 : time;

		// Adjust according to length of audio clip
		if( time > asset->getLength() ) {
			if( looping )
				time = std::fmod(time, asset->getLength());
			else
				time = asset->getLength();
		}

		if( active ) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSourcef(sourceId, AL_SEC_OFFSET, (ALfloat)time);
			ALUtility::checkErrors();
		}
		else {
			currentTime = time;
		}
	}


	double AudioInstance::getTime() {
		if( !active ) return currentTime;
		ALuint sourceId = ALData::instanceSourceMap.at(this);
		ALfloat time;
		alGetSourcef(sourceId, AL_SEC_OFFSET, &time);
		ALUtility::checkErrors();
		return time;
	}


	void AudioInstance::pause() {
		if( !paused && active ) {
			// Update current time to the time
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			ALfloat time;
			alGetSourcef(sourceId, AL_SEC_OFFSET, &time);
			alSourceStop(sourceId);
			currentTime = time;
			std::cout << "Pause time: " << currentTime << std::endl;
		}
		paused = true;
	}


	void AudioInstance::unpause() {
		paused = false;
	}


	bool AudioInstance::isPaused() {
		return paused;
	}


	void AudioInstance::restart() {
		if( active ) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSourcef(sourceId, AL_SEC_OFFSET, 0);
			ALUtility::checkErrors();
		}
		else {
			currentTime = 0;
		}
	}


	void AudioInstance::resume() {

	}


	void AudioInstance::stop() {
		AudioSystem::stopAudio(this);
	}


	void AudioInstance::onFinish(std::function<void(AudioInstance*)> callback) {
		onFinishCallback = callback;
	}

}