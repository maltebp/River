#pragma once

// Just a function for logging in debug build
#ifdef _DEBUG
#include <iostream>
#define LOG(x) std::cout << "ECS: " << x << std::endl
#else
#define LOG(x)
#endif


