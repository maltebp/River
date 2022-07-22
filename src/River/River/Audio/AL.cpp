#include "River/pch.h"

#include "AL.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

#include "River/Error.h"


namespace River {

	// Just a small helper function to convert the AL int enum to hex
	static std::string intToHex4(int i) {
		std::stringstream stream;
		stream << "0x" << std::setfill('0') << std::setw(4) << std::hex << i;
		return stream.str();
	}


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


	void AL::initialize() {
		if( initialized ) throw new InvalidStateException("Audio system has already been initialized");
		initialized = true;

		// Open default device
		ALCdevice* device = alcOpenDevice(nullptr);

		if( device == nullptr ) throw new AudioException("Could not open preffered audio device");

		// Not sure if we should check if eax2.0 exists here
		// The OpenAL programming guide does, but the return
		// result is never 

		// Open context
		ALCcontext* context = alcCreateContext(device, nullptr);
		alcMakeContextCurrent(context);
		checkContextError(device);

		// Set attenuation model
		alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
		checkErrors();

		// Generate sources
		for( int i = 0; i < NUM_SOURCES; i++ ) {
			ALuint sourceId;
			alGenSources(1, &sourceId);
			checkErrors();
			allSources[i] = sourceId;
			freeSources.push(allSources + i);
		}
	}


	bool AL::isInitialized() {
		return initialized;
	}


	ALuint* AL::getSource() {
		if( !initialized )
			throw new InvalidStateException("OpenAL has not been initialized");

		if( AL::freeSources.empty() )
			throw new InvalidStateException("No OpenAL sources are available when activating AudioInstance");
	
		ALuint* sourceId = AL::freeSources.top();
		AL::freeSources.pop();

		return sourceId;
	}


	void AL::releaseSource(ALuint* source) {
		if( !initialized )
			throw new InvalidStateException("OpenAL has not been initialized");
		AL::freeSources.push(source);
	}


	std::string AL::errorToString(ALenum errorCode) {
		switch (errorCode) {
			case AL_NO_ERROR: return "No error";
			case AL_INVALID_NAME: return "Invalid name";
			case AL_INVALID_ENUM: return "Invalid enum";
			case AL_INVALID_VALUE: return "Invalid value";
			case AL_INVALID_OPERATION: return "Invalid operation";
			case AL_OUT_OF_MEMORY: return "Out of memory";
		}
		return "Unknown error";
	}


	void AL::checkErrors() {
		ALenum error = alGetError();
		if (error == AL_NO_ERROR) return;

		// Collect all occured errors
		std::vector<ALenum> errors;
		errors.push_back(error);
		while ((error = alGetError()) != AL_NO_ERROR) {
			errors.push_back(error);
		}

		// Construct the error message
		std::stringstream msg;
		msg << errors.size();

		if (errors.size() > 1)
			msg << " AL errors occured: ";
		else
			msg << " AL error occured: ";

		for (int i = 0; i < errors.size(); i++) {
			msg << "'" << errorToString(errors[i]) << " (" << intToHex4(errors[i]) << ")'";
			if (i < (errors.size() - 1))
				msg << ", ";
		}

		throw new Exception(msg.str());
	}


	std::string AL::contextErrorToString(ALCenum errorCode) {
		switch (errorCode) {
			case ALC_NO_ERROR: return "No error";
			case ALC_INVALID_DEVICE: return "Invalid device";
			case ALC_INVALID_CONTEXT: return "Invalid context";
			case ALC_INVALID_ENUM: return "Invalid context";
			case ALC_INVALID_VALUE: return "Invalid value";
			case ALC_OUT_OF_MEMORY: return "Out of memory";
		}
		return "Unknown error";
	}


	void AL::checkContextError(ALCdevice* device) {
		if (device == nullptr) return;
		ALCenum error = alcGetError(device);
		if (error == ALC_NO_ERROR) return;

		// Collect all occured errors
		std::vector<ALCenum> errors;
		errors.push_back(error);
		while ((error = alGetError()) != ALC_NO_ERROR) {
			errors.push_back(error);
		}

		// Construct the error message
		std::stringstream msg;
		msg << errors.size();

		if (errors.size() > 1)
			msg << " ALC errors occured: ";
		else
			msg << " ALC error occured: ";

		for (int i = 0; i < errors.size(); i++) {
			msg << "'" << contextErrorToString(errors[i]) << " (" << intToHex4(errors[i]) << ")'";
			if (i < (errors.size() - 1))
				msg << ", ";
		}

		throw new AudioException(msg.str());
	}
}

