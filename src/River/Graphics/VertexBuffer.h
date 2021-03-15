#pragma once

#include "GL.h"
#include "Buffer.h"

namespace River {

	/**
	 * @brief	Wrapper class for an OpenGL buffer that is solely used
	 *			for the GL_ARRAY_BUFFER target
	*/
	class VertexBuffer : public Buffer {
	public:

		VertexBuffer(size_t initialSize = 0)
			:	Buffer(GL_ARRAY_BUFFER, GL_ARRAY_BUFFER_BINDING, initialSize)
		{}

	};

}


