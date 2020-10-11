#pragma once

#include <River.h>

using namespace River::ECS;



struct Transform : River::ECS::Component {
	double x = 0;
	double y = 0;
	River::Color color;
};