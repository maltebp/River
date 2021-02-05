#pragma once

#include "AudioAsset.h"

namespace River {

	class AudioSystem {
	public:
		static void initialize();

		// TODO: JUST FOR TESTING (WILL BE DELETED!)
		static void playAudio(AudioAsset* asset);


	private:
		inline static bool initialized = false;

	};

}