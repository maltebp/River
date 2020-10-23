#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <sstream>

#include "Signature.h"


namespace River::ECS {

	
	struct Entity;

	
	



	class SignatureArray {
	public:

		/**
		 * @param numSignaturesReserved	Number of signatures to reserve memory for at on creation
		*/
		SignatureArray(unsigned int initialMemorySize);
		~SignatureArray();

		/**
		 * @brief Removes all signatures from the array
		 *
		 * This deallocates the dynamic memory allocated for this list, and sets the number of
		 * contained signatures to 0.
		 * The signature size is not altered.
		*/
		void clear();

		/**
		 * @brief Checks if each signature in the array matches the given signature, and call the callback function in case it does
		 * @param signature
		 * @param callback
		*/
		void forMatchingSignatures(Signature& signature, std::function<void(unsigned int signatureIndex)> callback);


		void setSignatureBit(unsigned int signatureIndex, unsigned int bitIndex);
		bool getSignatureBit(unsigned int signatureIndex, unsigned int bitIndex);
		void unsetSignatureBit(unsigned int signatureIndex, unsigned int bitIndex);
		void unsetAllSignatureBit(unsigned int signatureIndex, unsigned int bitIndex);


		void setSignatureSize(unsigned int newSignatureSize);

		/**
		 * @brief Adds a new signature to the array, where all bits are set to 0
		 * @return	The index of the signature, which
		*/
		unsigned int add();


		/**
		 * @brief	Removes the signature on the given index, and moves the last signature in the array to this index
		 * @param signatureIndex	Index of the signature to remove (this index should have been returned by the add() method)
		 * @return	Index of the signature which was moved to the removed signatures slot (before it was removed), or 0
					if no signature was moved (because removed was either last element in list, or the only element)
		*/
		unsigned int remove(unsigned int signatureIndex);


		/**
		 * @brief Allocate enough space to hold the given number of signatures. If the memory required for this is
		 *			less than currently reserved memory, this function will do nothing.
		 * @param numSignatures		Number of signatures to reserver memory for
		*/
		void reserveSignatures(unsigned int numSignatures);

		/**
		 * @brief	Allocate enough memory to hold the given number of bytes. If the currently reserved memory
					is larger than this, the functill will do nothing.

		 * @param memory	Number of bytes to reserver
		*/
		void reserveMemory(unsigned int memory);




		/**
		 * @return	Number of signatures (not the reserved number)
		*/
		unsigned int getNumSignatures();

		
		/**
		 * @return	Number of bytes this array has reserved
		*/
		unsigned int getMemorySize();



	private:
		SignatureArray& operator=(SignatureArray& other) = delete;
		SignatureArray(const SignatureArray& other) = delete;


		/**
		 * @brief	Checks if the Signature query has been cached, and if it has, it runs the callback
					for each signature index in the cached query result

		 * @param signature		Signature to check for cache
		 * @param callback		Callback to run
		 * @return	True if a cached query was found and run, otherwise false
		*/
		bool checkCachedQuery(Signature& signature, std::function<void(unsigned int signatureIndex)> callback);



	private:

		double memoryStepSize = 0;

		/**
		 * @brief  Size of the allocated memory */
		unsigned int memorySize = 0;

		unsigned int numSignatures = 0;

		unsigned int signatureSize = 1;
		unsigned int signatureParts = 1; // Number of unsigned chars to represent elements (always elementSize ceil(elementSize/sizeof(unsigned char))

		unsigned char* data = nullptr;

		// Maps signature to index to Entity
		std::unordered_map<unsigned int, ECS::Entity*> entityMap; // TODO: Change to vector (faster lookup)

		// Maps Entity to signature index
		std::unordered_map<ECS::Entity*, unsigned int> indexMap;


		BitManipulator bitManipulator = BitManipulator(nullptr, 0);


		
		std::vector<std::pair<Signature, std::vector<unsigned int>>> cachedQueries;
		

	public:
		class IndexOutOfBoundsException : public Exception {
		public:
			IndexOutOfBoundsException(unsigned int index, unsigned maxIndex) :
				Exception("Signature index " + std::to_string(index) + " is out of bounds (max index is " + std::to_string(maxIndex) + ")") {
			}
		};


	};


}

