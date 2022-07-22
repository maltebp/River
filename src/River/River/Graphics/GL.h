#pragma once

// Graphics
#define GLEW_STATIC
#include "River/External/glew/glew.h"
#include "River/External/glfw/glfw3.h"


// GL Error handling
namespace River {

	namespace GL {

		void glCheckError();

	}

}

#if defined(_DEBUG) && !defined(GL)
	#define GL(x) x; River::GL::glCheckError()
#else
	#define GL(x) x
#endif