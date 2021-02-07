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


	AudioInstance::~AudioInstance(){
		// TODO: Kill the audio from the audio system here

		if( active )
			// TODO: Convert this to logging
			std::cout << "Audio is playing" << std::endl;
	}
	

	void AudioInstance::play() {
		AudioSystem::playAudio(this);
	}


	void AudioInstance::setVolume(double volume) {
		// Clamp volume to 0.0 - 1.0
		this->volume = volume > 1.0 ? 1.0 : (volume < 0.0 ? 0.0 : volume);
		if (active) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSourcef(sourceId, AL_GAIN, (ALfloat)this->volume);
		}
		dirty = true;
	}


	void AudioInstance::setPosition(double positionX, double positionY) {
		if (active) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSource3f(sourceId, AL_POSITION, (ALfloat)positionX, (ALfloat)positionY, 0);
			ALUtility::checkErrors();
		}
		this->positionX = positionX;
		this->positionY = positionY;
	}


	void AudioInstance::setVelocity(double velocityX, double velocityY) {
		if (active) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSource3f(sourceId, AL_VELOCITY, (ALfloat)velocityX, (ALfloat)velocityY, 0);
			ALUtility::checkErrors();
		}
		this->velocityX = velocityX;
		this->velocityY = velocityY;
	}


	void AudioInstance::set3D(bool toggle) {
		if (active && toggle != threeD) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			if (toggle) {
				alSourcei(sourceId, AL_SOURCE_RELATIVE, 0);
				alSource3f(sourceId, AL_POSITION, (ALfloat)positionX, (ALfloat)positionY, 0);
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
		if (active) {
			ALuint sourceId = ALData::instanceSourceMap.at(this);
			alSourcef(sourceId, AL_SEC_OFFSET, time);
			ALUtility::checkErrors();
		}
		else {
			currentTime = time;
		}	
	}


	double AudioInstance::getTime() {
		if (!active) return currentTime;
		ALuint sourceId = ALData::instanceSourceMap.at(this);
		ALfloat time;
		alGetSourcef(sourceId, AL_SEC_OFFSET, &time);
		return time;
	}


	void AudioInstance::pause() {
		if (!paused && active) {
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
		if (active) {
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

	}


	void AudioInstance::onFinish(std::function<void(AudioInstance*)> callback) {
		onFinishCallback = callback;
	}

}