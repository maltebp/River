#pragma once

#include "BitManipulator.h"


namespace River::ECS {


	class SignatureSizeReducedException : public Exception {
	public:
		SignatureSizeReducedException(int previousValue, int newValue) :
			Exception("Cannot reduce signature size (from " + std::to_string(previousValue) + " to " + std::to_string(newValue)) {
		}
	};

	/**
	 * @brief Data is heap allocated, so it's not efficient to copy this
	*/
	class Signature : public BitManipulator {
	public:

		Signature(unsigned int size);
		~Signature();

		Signature& operator=(const Signature& other);
		Signature(const Signature& other);

		/**
		 * @brief	Resizes the number of elements (bits) this signature may hold. This may reallocate the signatures memory, in
					case the current allocated memory can't hold the new number of elements
		 * @param newSize	New number of elements (must be higher than existing)
		*/
		void resize(unsigned int newSize);

	};
}



