#pragma once

#include <vector>

#include "River/Primitives/Resolution.h"


namespace River {

	class Screen {
	public:

		static Resolution getResolution();

		static std::vector<Resolution> getSupportedResolutions();
		
	};

}


