#pragma once

#include <vector>

#include "River/Primitives/Resolution.h"


namespace River {

	class Screen {
	public:

		static Resolution getResolution();

		// Doc notes:
		//  - Also returns Resolutions that are higher than current
		static std::vector<Resolution> getSupportedResolutions();
		
	};

}


