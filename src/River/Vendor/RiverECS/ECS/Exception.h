#pragma once

#include <stdexcept>


namespace River::ECS {
	class Exception : public std::runtime_error {
	public:
		using std::runtime_error::runtime_error;
	};


	class MemoryAllocationException : public Exception {
	public:
		MemoryAllocationException(unsigned int memorySize) :
			Exception("Memory allocation failed when trying to allocate " + std::to_string(memorySize) + " bytes") {
		}
	};

}


