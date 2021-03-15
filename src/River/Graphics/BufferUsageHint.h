#pragma once

#include "GL.h"

namespace River {

	enum class BufferUsageHint {
		STATIC = GL_STATIC_DRAW,
		DYNAMIC = GL_DYNAMIC_DRAW
	};

}