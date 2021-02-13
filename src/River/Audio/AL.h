#pragma once

#include "River/Vendor/OpenAL/al.h"
#include "River/Vendor/OpenAL/alc.h"

#include <string>
#include <stack>

namespace River {
	
	class AL {
	public:

		// The number of AL sources to create. This is in accordance with
		// the expected minimum number of sounds that may play at the same
		// time
		static inline const int NUM_SOURCES = 16;

		static void initialize();

		static bool isInitialized();

		static ALuint* getSource();

		static void releaseSource(ALuint*);

		static std::string errorToString(ALenum errorCode);

		static void checkErrors();

		static std::string contextErrorToString(ALCenum errorCode);

		static void checkContextError(ALCdevice* device);

	private:

		static inline bool initialized = false;

		static inline ALuint allSources[NUM_SOURCES];
		
		static inline std::stack<ALuint*> freeSources;
	};

}
