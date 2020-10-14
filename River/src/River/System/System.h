#pragma once

#include "River/Vendor/RiverECS/ECS.h"

namespace River {

	/**
	 * @brief	An generic ECS system class, which can be updated
	*/
	class System {
	public:
		virtual void update(ECS::Domain& domain) = 0;
	};

}


