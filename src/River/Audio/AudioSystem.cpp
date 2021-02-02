
#include "AudioSystem.h"

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include "River.h"
#include "River/Vendor/OpenAL/al.h"
#include "River/Vendor/OpenAL/alc.h"

#define AL(x) x; alCheckError();

namespace River {

	// Just a small helper function to convert the AL int enum to hex
	static std::string intToHex4(int i) {
		std::stringstream stream;
		stream << "0x" << std::setfill('0') << std::setw(4) << std::hex << i;
		return stream.str();
	}


	static std::string alErrorToString(ALenum errorCode) {
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


	void alCheckError() {
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
			msg << "'" << alErrorToString(errors[i]) << " (" << intToHex4(errors[i]) << ")'";
			if (i < (errors.size() - 1))
				msg << ", ";
		}

		throw new AudioException(msg.str());
	}


	static std::string alcErrorToString(ALenum errorCode) {
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


	void alcCheckError(ALCdevice* device) {
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
			msg << "'" << alcErrorToString(errors[i]) << " (" << intToHex4(errors[i]) << ")'";
			if (i < (errors.size() - 1))
				msg << ", ";
		}

		throw new AudioException(msg.str());
	}



	struct OpenALData {
		ALCdevice* device;

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
	}


	// Just a small helper function to convert int to hex
	static std::string errorToHex(GLenum i) {
		std::stringstream stream;
		stream << "0x" << std::setfill('0') << std::setw(4) << std::hex << i;
		return stream.str();
	}

}



