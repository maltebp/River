#pragma once

#include "River/Vendor/OpenAL/al.h"
#include "River/Vendor/OpenAL/alc.h"

#include <string>


namespace River {
	
	class ALUtility {
	public:

		static std::string errorToString(ALenum errorCode);

		static void checkError();

		static std::string contextErrorToString(ALCenum errorCode);

		static void checkContextError(ALCdevice* device);
	};

}
