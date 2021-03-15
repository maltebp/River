
#include <vector>


namespace River {


	/**
	 * @brief	Array intended to be used for storing OpenGL buffer data
	 *			before uploading it to the GPU.
	*/
	class MixedArray {
	public:

		void reserve(size_t bytes) {
			data.reserve(bytes);
		}

		
		/**
		 * @brief	Reserve memory for current size + extra bytes
		*/
		void reserveExtra(size_t bytes) {
			data.reserve(data.size() + bytes);	
		}


		/**
		 * @brief	Pushes back an element of the given type to the array
		*/
		template <typename ... T>
		void add(T ... t) {
			(push_back_internal(t), ...);
		}


		void clear() {
			data.clear();
		}


		/**
		 * @return	Returns pointer to the data
		*/
		void* getPointer() {
			return data.data();
		}


		/**
		 * @return	Number of bytes stored in the array
		*/
		size_t getBytes() {
			return data.size();
		}


		/**
		 * @brief	Reference to inner vector
		*/
		const std::vector<uint8_t>& getVector() {
			return data;
		}


	private:

		template <typename T>
		void push_back_internal(T t) {
			size_t bytes = sizeof(t);

			size_t currentOffset = data.size();

			data.resize(data.size() + bytes);

			T* rawData = reinterpret_cast<T*>(data.data() + currentOffset);

			*rawData = t;
		}

	private:

		std::vector<uint8_t> data;

	};

}