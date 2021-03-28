#include "River/pch.h"

#include "VertexArray.h"

#include "River/Error.h"
#include "River/Utility/String.h"


namespace River {
	

	VertexArray::VertexArray() {
		GL(glGenVertexArrays(1, &id));
	}


	VertexArray::~VertexArray() {
		// Delete will automatically unbind, and silently ignore
		// if the id is not valid (including 0)
		GL(glDeleteVertexArrays(1, &id));
	}


	void VertexArray::setAttribute(unsigned int index, const VertexAttribute& type, VertexBuffer* buffer, size_t bufferOffset, size_t spacing) {
		
		if( type == VertexAttributes::NONE ) {
			throw InvalidArgumentException("Attribute type must not VertexAttribute::NONE");
		}

		if( buffer == nullptr ) {
			throw InvalidArgumentException("Vertex buffer must not be nullptr");
		}

		GLint maxVertexAttributes;
		GL(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes));
		if( (GLint)index >= maxVertexAttributes ) {
			throw InvalidArgumentException("Attribute index must be less than " + std::to_string(maxVertexAttributes) + " on this platform");
		}

		// Get current buffer and array, for rebinding afterwards
		GLint currentVertexArray = 0;
		GLint currentVertexBuffer = 0;
		GL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVertexArray));
		GL(glGetIntegerv(GL_VERTEX_ARRAY_BUFFER_BINDING, &currentVertexBuffer));

		// Bind this vertex array 
		GL(glBindVertexArray(id));
	
		buffer->bind();

		// Assign the attribute
		switch( type.getType() ) {
		case GL_BYTE: case GL_UNSIGNED_BYTE: case GL_SHORT: case GL_UNSIGNED_SHORT: case GL_INT: case GL_UNSIGNED_INT: {
			GL(glVertexAttribIPointer(index, type.getCount(), type.getType(), (GLsizei)(type.getTypeSize() * type.getCount() + spacing), (void*)bufferOffset));
		} break;
		case GL_HALF_FLOAT: case GL_FLOAT: case GL_DOUBLE: case GL_FIXED: case GL_INT_2_10_10_10_REV: case GL_UNSIGNED_INT_2_10_10_10_REV: case GL_UNSIGNED_INT_10F_11F_11F_REV: {
			GL(glVertexAttribPointer(index, type.getCount(), type.getType(), GL_FALSE, (GLsizei)(type.getTypeSize() * type.getCount() + spacing), (void*)bufferOffset));
		} break;
		default: {
			throw new InvalidArgumentException("The GL enum " + toHexString(type.getType()) + " is not supported as a vertex array attribute type.");
		}};
		GL(glEnableVertexAttribArray(index));

		numAttributesSet++;

		// Rebind old buffer/array
		GL(glBindVertexArray(currentVertexArray));
		GL(glBindBuffer(GL_ARRAY_BUFFER, currentVertexBuffer));
	}

	
	void VertexArray::setIndexBuffer(const IndexBuffer* indexBuffer, const VertexIndexType& indexType) {

		if( indexType == VertexIndexTypes::NONE ) {
			throw InvalidArgumentException("Index type must not be VertexIndexType::NONE");
		}

		this->indexBuffer = indexBuffer;
		this->indexType = indexType;

		// Get current buffer and array, for rebinding afterwards
		GLint currentVertexArray = 0;
		GLint currentVertexBuffer = 0;
		GL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVertexArray));
		GL(glGetIntegerv(GL_VERTEX_ARRAY_BUFFER_BINDING, &currentVertexBuffer));

		// Bind this vertex array 
		GL(glBindVertexArray(id));

		indexBuffer->bind();

		// Rebind old buffer/array
		GL(glBindVertexArray(currentVertexArray));
		GL(glBindBuffer(GL_ARRAY_BUFFER, currentVertexBuffer));
	}


	void VertexArray::bind() const {
		if( numAttributesSet == 0 ) {
			throw new InvalidStateException("Vertex Array has no attributes set");
		}
		GL(glBindVertexArray(id));
	}


	bool VertexArray::isBound() const {
		GLint currentVertexArray;
		GL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVertexArray));
		return currentVertexArray == id;
	}


	void VertexArray::drawTriangles(unsigned int count, unsigned int offset) {
		if( count == 0 ) {
			return;
		}

		bind();

		if( indexBuffer != nullptr ) {
			size_t byteOffset = ((size_t)offset * indexType.getTypeSize());
			GL(glDrawElements(GL_TRIANGLES, count*3, indexType.getType(), (void*)byteOffset));
		}
		else {
			GL(glDrawArrays(GL_TRIANGLES, (GLint)offset, count * 3));
		}

	}


	void VertexArray::drawLines(unsigned int count, unsigned int offset) {
		bind();

		if( indexBuffer != nullptr ) {
			size_t byteOffset = ((size_t)offset * indexType.getTypeSize());
			GL(glDrawElements(GL_LINES, count * 2, indexType.getType(), (void*)byteOffset));
		}
		else {
			GL(glDrawArrays(GL_LINES, offset, count * 2));
		}

	}


	GLuint VertexArray::getId() const {
		return id;
	}

}