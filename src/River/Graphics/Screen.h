#pragma once

#include <vector>

#include "River/Primitives/Resolution.h"


namespace River {

	/**
	 * @brief	Represents the primary screen/monitor of the executing device
	*/
	class Screen {
	public:

		static Resolution getResolution();

		// Doc notes:
		//  - Also returns Resolutions that are higher than current
		static std::vector<Resolution> getSupportedResolutions();
		
	};

}


