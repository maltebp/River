#pragma once

#include "Shader.h"
#include "River/Graphics/GL.h"


namespace River {

	class VertexShader : public Shader {
	public:

		VertexShader(const std::string& source)
			: Shader(GL_VERTEX_SHADER, source) 		{
		}

	};

}