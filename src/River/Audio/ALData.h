#pragma once

#include <unordered_map>
#include <stack>

#include "River/Vendor/OpenAL/al.h"
#include "AudioInstance.h"

namespace River {
	
	// Static AL data used by the AudioSystem
	// Not placed in header to abstract OpenAL away from end user
	class ALData {
	public:
		// The number of AL sources to create. This is in accordance with
		// the expected minimum number of sounds that may play at the same
		// time
		static inline const int NUM_SOURCES = 16;

		static inline std::unordered_map<AudioInstance*, ALuint> instanceSourceMap;

		static inline std::stack<ALuint> freeSources;

		static inline std::vector<ALuint> allSources;
	};

}


