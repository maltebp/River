#include "River/pch.h"

#include "Buffer.h"

#include "River/Error.h"


// Macro for use in VertexBuffer class, to bind the buffer
// and rebind the previous buffer after function
#define TEMP_BIND(x) GLint _currentBufferId;										\
					 glGetIntegerv(this->queryTarget, &_currentBufferId);			\
					 if( this->id != _currentBufferId ) {							\
						GL(glBindBuffer(this->target, id));							\
					 }																\
					 x;																\
					 if( this->id != _currentBufferId ) {	\
						GL(glBindBuffer(this->target, _currentBufferId));			\
					 }	

namespace River {


	Buffer::Buffer(GLenum target, GLenum queryTarget, size_t initialSize, BufferUsageHint usageHint ) {
		GL(glGenBuffers(1, &id));

		this->target = target;
		this->queryTarget = queryTarget;
		this->usageHint = usageHint;
		size = initialSize;

		if( initialSize == 0 ) return;

		// It's possible to allocate buffer memory, without initializing
		// it, but for simplicity we just initialize (or reset(..) does)
		reset(initialSize);
	}


	Buffer::~Buffer() {
		// Delete will automatically unbind, and silently ignore
		// if the buffer id is not valid (including 0)
		GL(glDeleteBuffers(1, &id));
	}


	// TODO: Convert setting of data into 1 function
	/*	Parameters:
		 - Offset
	 	 - Data
	 	 - Bytes
		 - Fit
		
		"Algorithm":

			Start, within size		-> Sub data
		    +++++++++
			--------------

			Start, within size		-> Sub data
			++++++++++++++
			--------------

			Start, outside size		-> Data
			+++++++++++++++++++
			--------------

			Offset, within size		-> Sub data
				++++++++
			--------------

			THIS ONE IS THE PROBLEM!
			Offset, outside size	-> Copy sub data, 
			        +++++++++
			--------------

	*/


	void Buffer::setData(void* data, size_t bytes, bool fit) {	

		if( bytes > size || (bytes < size && fit) ) {
			// Reallocate and copy
			TEMP_BIND(
				GL(glBufferData(target, bytes, data, static_cast<GLenum>(this->usageHint)));
			)
			this->size = bytes;
		}
		else {
			// No reallocation
			TEMP_BIND(
				GL(glBufferSubData(target, 0, bytes, data));
			)
		}
	}


	void Buffer::setSubData(void* data, size_t bytes, size_t targetOffset) {
		if( bytes > size - targetOffset ) {
			throw InvalidArgumentException("Vertex buffer sub data target exceeds the target limit ");
		}

		TEMP_BIND(
			GL(glBufferSubData(target, targetOffset, bytes, data));
		)
	}


	void Buffer::reset(size_t newSize) {

		if( newSize == 0 && this->size > 0) {
			// Free the memory
			TEMP_BIND(
				GL(glBufferData(target, 0, nullptr, static_cast<GLenum>(this->usageHint)));
			)
		}
		else {
			// Reallocate memory and set to all zeros
			void* data = calloc(newSize, 1);
			TEMP_BIND(
				GL(glBufferData(target, newSize, data, static_cast<GLenum>(this->usageHint)));
			)
			free(data);
		}

		this->size = newSize;
	}

	
	void Buffer::bind() const {
		GL(glBindBuffer(target, id));
	}


	void Buffer::unbind() const {
		if( isBound() ) {
			GL(glBindBuffer(target, 0));
		}
	}


	bool Buffer::isBound() const {
		GLint currentBufferId;
		GL(glGetIntegerv(queryTarget, &currentBufferId));
		return currentBufferId == id;
	}


	size_t Buffer::getSize() const {
		return size;
	}


	GLuint Buffer::getId() const {
		return id;
	}


}