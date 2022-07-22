#pragma once

#include "GL.h"


namespace River {

	
	struct VertexAttributes;


	/**
	 * @brief	Describes a type of Vertex Attribute (i.e. a float3)
	*/
	struct VertexAttribute {
		friend VertexAttributes;
	public:


		VertexAttribute()
			: type(0), typeSize(0), count(0)
		{}


		GLenum getType() const {
			return type;
		}


		GLsizei getTypeSize() const {
			return typeSize;
		}

		GLint getCount() const {
			return count;
		}

		
		bool operator ==(const VertexAttribute& other) const {
			return type == other.type && typeSize == other.typeSize && count == other.count;
		}


	private:


		VertexAttribute(GLenum type, GLsizei typeSize, GLint count)
			: type(type), typeSize(typeSize), count(count)
		{}


	private:

		GLenum type;

		GLsizei typeSize;

		GLint count;

	};


	/**
	 * @brief	Valid data types for Vertex Attributes in a VertexArray
	*/
	struct VertexAttributes {

		/**
		 * @brief Same as default constructed VertexAttribute
		*/
		static inline const VertexAttribute		NONE	= { 0, 0, 0 };

		static inline const VertexAttribute		FLOAT	= { GL_FLOAT, sizeof(GLfloat), 1 };
		static inline const VertexAttribute		FLOAT2	= { GL_FLOAT, sizeof(GLfloat), 2 };
		static inline const VertexAttribute		FLOAT3	= { GL_FLOAT, sizeof(GLfloat), 3 };
		static inline const VertexAttribute		FLOAT4	= { GL_FLOAT, sizeof(GLfloat), 4 };

		static inline const VertexAttribute		DOUBLE	= { GL_DOUBLE, sizeof(GLdouble), 1 };
		static inline const VertexAttribute		DOUBLE2 = { GL_DOUBLE, sizeof(GLdouble), 2 };
		static inline const VertexAttribute		DOUBLE3 = { GL_DOUBLE, sizeof(GLdouble), 3 };
		static inline const VertexAttribute		DOUBLE4 = { GL_DOUBLE, sizeof(GLdouble), 4 };

		static inline const VertexAttribute		INT		= { GL_INT, sizeof(GLint), 1 };
		static inline const VertexAttribute		INT2	= { GL_INT, sizeof(GLint), 2 };
		static inline const VertexAttribute		INT3	= { GL_INT, sizeof(GLint), 3 };
		static inline const VertexAttribute		INT4	= { GL_INT, sizeof(GLint), 4 };

		static inline const VertexAttribute		UINT	= { GL_UNSIGNED_INT, sizeof(GLuint), 1 };
		static inline const VertexAttribute		UINT2	= { GL_UNSIGNED_INT, sizeof(GLuint), 2 };
		static inline const VertexAttribute		UINT3	= { GL_UNSIGNED_INT, sizeof(GLuint), 3 };
		static inline const VertexAttribute		UINT4	= { GL_UNSIGNED_INT, sizeof(GLuint), 4 };

		static inline const VertexAttribute		SHORT	= { GL_SHORT, sizeof(GLshort), 1 };
		static inline const VertexAttribute		SHORT2	= { GL_SHORT, sizeof(GLshort), 2 };
		static inline const VertexAttribute		SHORT3	= { GL_SHORT, sizeof(GLshort), 3 };
		static inline const VertexAttribute		SHORT4	= { GL_SHORT, sizeof(GLshort), 4 };

		static inline const VertexAttribute		BYTE	= { GL_BYTE, sizeof(GLbyte), 1 };
		static inline const VertexAttribute		BYTE2	= { GL_BYTE, sizeof(GLbyte), 2 };
		static inline const VertexAttribute		BYTE3	= { GL_BYTE, sizeof(GLbyte), 3 };
		static inline const VertexAttribute		BYTE4	= { GL_BYTE, sizeof(GLbyte), 4 };

		static inline const VertexAttribute		UBYTE	= { GL_UNSIGNED_BYTE, sizeof(GLubyte), 1 };
		static inline const VertexAttribute		UBYTE2	= { GL_UNSIGNED_BYTE, sizeof(GLubyte), 2 };
		static inline const VertexAttribute		UBYTE3	= { GL_UNSIGNED_BYTE, sizeof(GLubyte), 3 };
		static inline const VertexAttribute		UBYTE4	= { GL_UNSIGNED_BYTE, sizeof(GLubyte), 4 };

	};


}

