#include "River/pch.h"

#include "AudioListener.h"

#include "River/Error.h"
#include "AL.h"


namespace River {


	void AudioListener::setVolume(double volume) {
		if( volume < 0 )
			throw new InvalidArgumentException("Master volume may not be less than 0");
		AudioListener::volume = volume;
		alListenerf(AL_GAIN, (ALfloat)volume);
		AL::checkErrors();
	}


	double AudioListener::getVolume() {
		return volume;
	}


	void AudioListener::setPosition(double positionX, double positionY) {
		AudioListener::positionX = positionX;
		AudioListener::positionY = positionY;
		alListener3f(AL_POSITION, (ALfloat)positionX, (ALfloat)positionY, (ALfloat)-depth);
		AL::checkErrors();
	}


	double AudioListener::getPositionX() {
		return positionX;
	}

	double AudioListener::getPositionY() {
		return positionY;
	}


	void AudioListener::setVelocity(double velocityX, double velocityY) {
		AudioListener::velocityX = velocityX;
		AudioListener::velocityY = velocityY;
		alListener3f(AL_VELOCITY, (ALfloat)velocityX, (ALfloat)velocityY, 0);
		AL::checkErrors();
	}


	double AudioListener::getVelocityX() {
		return velocityX;
	}

	
	double AudioListener::getVelocityY() {
		return velocityY;
	}


	void AudioListener::setDepth(double depth) {
		AudioListener::depth = depth;
		alListener3f(AL_POSITION, (ALfloat)positionX, (ALfloat)positionY, (ALfloat)-depth);
		AL::checkErrors();
	}


	double AudioListener::getDepth() {
		return depth;
	}


	void AudioListener::setRotation(double rotation) {
		rotation = fmod(rotation, 360);
		if( rotation < 0 ) rotation += 360;
		AudioListener::rotation = rotation;

		// Adjust rotation by adding 90 degrees, to make 0 degrees
		// point upwards, and convert it to radians in range -PI to PI
		double radianRotation;
		radianRotation = AudioListener::rotation + 90;
		if( radianRotation >= 360 ) radianRotation -= 360; // Reclamp to 0-360
		if( radianRotation > 180 ) radianRotation -= 360; // Adjust to -180 to 108
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
		AL::checkErrors();
	}


	double AudioListener::getRotation() {
		return rotation;
	}

}