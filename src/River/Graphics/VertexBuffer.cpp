#include "River/pch.h"

#include "VertexBuffer.h"

#include "River/Error.h"

// Macro for use in VertexBuffer class, to bind the buffer
// and rebind the previous buffer after function
#define TEMP_BIND(x) GLint _currentBufferId;							\
					 glGetIntegerv(GL_ARRAY_BUFFER, &_currentBufferId);	\
					 if( this->id != _currentBufferId ) {				\
						glBindBuffer(GL_ARRAY_BUFFER, id);				\
					 }													\
					 x													\
					 if( this->id != _currentBufferId ) {				\
						glBindBuffer(GL_ARRAY_BUFFER, _currentBufferId);\
					 }		


namespace River {
	

	VertexBuffer::VertexBuffer(size_t initialSize) {
		GL(glGenBuffers(1, &id));
		
		size = initialSize;

		if( initialSize == 0 ) return;

		// It's possible to allocate buffer memory, without initializing
		// it, but for simplicity we just initialize (or reset(..) does)
		reset(initialSize);
	}


	VertexBuffer::~VertexBuffer() {
		// Delete will automatically unbind, and silently ignore
		// if the buffer id is not valid (including 0)
		GL(glDeleteBuffers(1, &id));
	}


	void VertexBuffer::setData(void* data, size_t bytes, bool fit) {	
		this->size = size;

		if( bytes > size || (bytes < size && fit) ) {
			// Reallocate and copy
			TEMP_BIND(
				GL(glBufferData(GL_ARRAY_BUFFER, bytes, data, GL_DYNAMIC_DRAW));
			)
		}
		else {
			// No reallocation
			TEMP_BIND(
				GL(glBufferSubData(GL_ARRAY_BUFFER, 0, bytes, data));
			)
		}
	}


	void VertexBuffer::setSubData(void* data, size_t bytes, size_t targetOffset) {
		if( bytes > size - targetOffset ) {
			throw InvalidArgumentException("Vertex buffer sub data target exceeds the target limit ");
		}

		TEMP_BIND(
			GL(glBufferSubData(GL_ARRAY_BUFFER, targetOffset, bytes, data));
		)
	}


	void VertexBuffer::reset(size_t newSize) {

		if( newSize == 0 && this->size > 0) {
			// Free the memory
			TEMP_BIND(
				GL(glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW));
			)
		}
		else {
			// Reallocate memory and set to all zeros
			void* data = calloc(newSize, 1);
			TEMP_BIND(
				GL(glBufferData(GL_ARRAY_BUFFER, newSize, data, GL_DYNAMIC_DRAW));
			)
			free(data);
		}

		this->size = newSize;
	}

	
	void VertexBuffer::bind() {
		GL(glBindBuffer(GL_ARRAY_BUFFER, id));
	}


	void VertexBuffer::unbind() {
		if( isBound() ) {
			GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}
	}


	bool VertexBuffer::isBound() {
		GLint currentBufferId;
		GL(glGetIntegerv(GL_ARRAY_BUFFER, &currentBufferId));
		return currentBufferId == id;
	}


	size_t VertexBuffer::getSize() {
		return size;
	}


	GLuint VertexBuffer::getId() {
		return id;
	}

}
