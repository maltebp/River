#pragma once

#include "Shader.h"
#include "River/Graphics/GL.h"


namespace River {

	class FragmentShader : public Shader {
	public:

		FragmentShader(const std::string& source) 
			: Shader(GL_FRAGMENT_SHADER, source)
		{}
	
	};

}