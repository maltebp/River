#pragma once

#include "GL.h"

namespace River {


	/**
	 * @brief	Wrapper class for an OpenGL buffer object
	*/
	class Buffer {
	public:

		/**
		 * @brief	Constructs a new GL buffer objects
		 *
		 * @param initialSize	If none zero, the given size will be allocated on creation,
		 *						and occupied with all zeros
		*/
		Buffer(GLenum target, GLenum queryTarget, size_t initialSize = 0);

		~Buffer();

		/**
		 * @brief	Sets the data of the buffer
		 *			If the buffer is not large enough to hold the data (bytes > buffer size),
		 *			a reallocation will occur.
		 *
		 * @param data			Pointer to data to copy to the buffer
		 * @param bytes			Number of bytes to copy from the data array
		 * @param fit			Whether the allocated memory should be shrunk to fit the new data.
		 *						Only has an effect if 'bytes' is less than the current buffer size.
		*/
		void setData(void* data, size_t bytes, bool fit = false);

		/**
		 * @brief	Sets the data of the buffer, to the data of the given vector.
					If the buffer is not large enough, to hold the data (sizeof(T)*data.size() >
		 *			buffer size), a reallocation of the buffer will occur.
		 *
		 * @tparam T	Type of the vector's data. Remember possible data alignment in case T is a struct or class
		 * @param data	Vector which data is copied into the buffer
		 * @param fit	Whether the allocated memory should be shrunk to fit the new data.
		 *				Only has an effect if 'bytes' is less than the current buffer size.
		*/
		template <typename T>
		void setData(const std::vector<T>& data, bool fit = false) {
			setData((void*)data.data(), sizeof(T) * data.size(), fit);
		}

		/**
		 * @brief	Sets a portion of the buffer's data to the given data. This will never perform
		 *			a reallocation
		 *
		 * @param data			Pointer to data to copy to the buffer
		 * @param bytes			Number of bytes to copy from the data array
		 * @param targetOffset	Byte offset into buffer's data to copy into
		 *
		 * @throws	River::InvalidArgumentException		If the target exceeds the size of the buffer
		 *												(targetOffset + bytes > buffer size)
		*/
		void setSubData(void* data, size_t bytes, size_t targetOffset);

		/**
		 * @brief	Sets a portion of the buffer's data to the given data. This will never perform
		 *			a reallocation
		 *
		 * @tparam T			Type of the vector's data. Remember possible data alignment
								in case T is a struct or class
		 * @param data			Pointer to data to copy to the buffer
		 * @param bytes			Number of bytes to copy from the data array
		 * @param targetOffset	Byte offset into buffer's data to copy into
		 *
		 * @throws	River::InvalidArgumentException		If the target exceeds the size of the buffer
		 *												(targetOffset + bytes > buffer size)
		*/
		template <typename T>
		void setSubData(const std::vector<T>& data, size_t targetOffset) {
			setSubData((void*)data.data(), sizeof(T) * data.size(), targetOffset);
		}

		// NOTE:
		// More variations of setData and setSubData are not defined simplicity, but
		// may be later introduced if found useful

		/**
		 * @brief	Resizes the buffer to the given size, and sets the data to all zeros.
		*/
		void reset(size_t size);


		/**
		 * @brief	Binds this buffer to its given target
		*/
		void bind() const;

		/**
		 * @brief	Unbinds this buffer, if it is currently bound
		*/
		void unbind() const;

		/**
		 * @return	True if this buffer is bound. It may have been unbound, even
		 *			if unbind has never manually been called, in case another
		 *			buffer was bound.
		*/
		bool isBound() const;

		/**
		 * @return	Size of the buffer in bytes
		*/
		size_t getSize() const;

		/**
		 * @return	The unique GL buffer id for this buffer. This will persist
		 *			throughouts the instance's lifetime
		*/
		GLuint getId() const;


	private:

		Buffer(const Buffer& other) = delete;

		Buffer& operator=(const Buffer& other) = delete;


	protected:

		GLuint id;

		size_t size;

		GLenum target;

		GLenum queryTarget;

	};

}


