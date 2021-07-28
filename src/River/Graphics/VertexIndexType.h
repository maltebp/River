#pragma once

#include "GL.h"


namespace River {

	struct VertexIndexTypes;


	/**
	 * @brief	Describes a type of vertex index (i.e. unsigned int)
	*/
	struct VertexIndexType {
		friend VertexIndexTypes;
	public:


		VertexIndexType()
			: type(0), typeSize(0)
		{ }


		GLenum getType() const {
			return type;
		}


		GLsizei getTypeSize() const {
			return typeSize;
		}


		bool operator ==(const VertexIndexType& other) const {
			return type == other.type && typeSize == other.typeSize;
		}


	private:


		VertexIndexType(GLenum type, GLsizei typeSize)
			: type(type), typeSize(typeSize)
		{}


	private:

		GLenum type;

		GLsizei typeSize;

	};


	/**
	 * @brief	Valid data types for Vertex Attributes in a VertexArray
	*/
	struct VertexIndexTypes {

		/**
		 * @brief Same as default constructed VertexIndexType
		*/
		static inline const VertexIndexType		NONE = { 0, 0 };

		static inline const VertexIndexType		UBYTE	= { GL_UNSIGNED_BYTE, sizeof(GLubyte) };
		static inline const VertexIndexType		USHORT	= { GL_UNSIGNED_SHORT, sizeof(GLushort) };
		static inline const VertexIndexType		UINT	= { GL_UNSIGNED_INT, sizeof(GLuint) };

	};


}