#pragma once

namespace River {

	class AudioSystem {
	public:
		static void initialize();


	private:
		inline static bool initialized = false;

		// Pointer to native data (defined in implementing .cpp file)
		inline static void* nativeData = nullptr;
	};

}