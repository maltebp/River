#pragma once

// Graphics
#define GLEW_STATIC
#include <River/Vendor/GL/glew.h>
#include <River/Vendor/GLFW/glfw3.h>


// GL Error handling
namespace River {
	namespace GLError {
		void glCheckError();
	}
}

#if defined(_DEBUG) && !defined(GL)
	#define GL(x) x; River::GLError::glCheckError()
#else
	#define GL(x) x;
#endif