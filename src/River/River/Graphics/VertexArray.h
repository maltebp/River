#pragma once

#include "GL.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexAttribute.h"
#include "VertexIndexType.h"


namespace River {


	/**
	 * @brief	Utility and wrapper class for an OpenGL Vertex Array Object
	*/
	class VertexArray {
	public:

		/**
		 * @brief	This will generate a new OpenGL buffer id, so the GL context must be set
		*/
		VertexArray();

		~VertexArray();

		/**
		 * @brief	Sets one of the attributes of this vertex array
		 * 
		 * @param index	Index of attribute to set. Max index may vary from platform to platform, but 15 is certain.
		 * @param type	Data type of the attribute to set. Must not be VertexAttribute::NONE
		 * @param buffer	Vertex buffer object this attributes should source its data from. The buffer does not have to
		 *					be initialized with any data when setting it. Must not be nullptr.
		 * @param bufferOffset	Byte offset into the buffer from which to source the first vertex
		 * @param spacing	Number of bytes between the end of one vertex to the beginning of next. A value of 0 means 
		 *					the vertices are placed with no gaps in between
		*/
		void setAttribute(unsigned int index, const VertexAttribute& type, VertexBuffer* buffer, size_t bufferOffset = 0, size_t spacing = 0);

		/**
		 * @brief	Sets the index buffer to be used by this Vertex Array.
		 *			
		 * @param buffer 
		*/
		void setIndexBuffer(const IndexBuffer* buffer, const VertexIndexType& indexType);

		/**
		 * @brief	Binds this vertex array for use
		*/
		void bind() const;

		/**
		 * @return	Whether this is the currently bound vertex array
		*/
		bool isBound() const;

		/**
		 * @brief	Perform a draw call using GL_TRIANGLES and this vertex array.
		 *			glDrawElements is used if an index buffer has been set, other
	     *			wise glDrawArrays is used
		 * 
		* @param count		Number of triangles to draw. Each triangle uses 3 vertices
		 * @param offset	Offset into the vertices (or indices if an index buffer is attached)
		*/
		void drawTriangles(size_t count, size_t offset = 0) const;

		/**
		 * @brief	Perform a draw call using GL_LINES and this vertex array.
		 *			glDrawElements is used if an index buffer has been set, other
		 *			wise glDrawArrays is used
		 *
		 * @param count		Number of lines to draw. Each line uses 2 vertices/indices
		 * @param offset	Offset into the vertices/indices if an index buffer is attached
		*/
		void drawLines(size_t count, size_t offset = 0) const;

		/**
		 * @return	The GL id for the this Vertex Array
		*/
		GLuint getId() const;

	
	private:
		
		VertexArray(const VertexArray&) = delete;

		// This could be implemented at some point
		VertexArray(VertexArray&&) = delete;


	private:

		GLuint id;

		unsigned int numAttributesSet = 0;

		bool built = false;

		const IndexBuffer* indexBuffer = nullptr;

		VertexIndexType indexType = VertexIndexTypes::NONE;
			
	};

}


