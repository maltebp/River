
#pragma once

#include "GL.h"
#include "Buffer.h"

namespace River {

	/**
	 * @brief	Wrapper class for an OpenGL buffer that is solely used
	 *			for the GL_ELEMENT_ARRAY_BUFFER target
	*/
	class IndexBuffer : public Buffer {
	public:

		IndexBuffer(size_t initialSize = 0, BufferUsageHint usageHint = BufferUsageHint::DYNAMIC)
			: Buffer(GL_ELEMENT_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER_BINDING, initialSize, usageHint) 		{
		}

	};

}