#pragma once

#include "River/Vendor/RiverECS/ECS.h"

namespace River::ECS {


	struct Transform : Component {
		double x = 0;
		double y = 0;
		double depth = 0;
		double width = 0;
		double height = 0;
		
	};

}