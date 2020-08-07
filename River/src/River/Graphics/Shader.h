#pragma once

#include <string>

namespace River {

	class Shader {
		
	private:
		unsigned int id;
		std::string source;

		Shader();

	public:

		/**
		 * @param type The OpenGL shader type (i.e. GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
		 * @param source The source code for the shader
		 * @throws River::ShaderException Thrown if someone goes wrong when loading the shader (i.e. source code couldn't compile)
		*/
		Shader(unsigned int type, const std::string &source);
		~Shader();

		unsigned int getId();

	};

}

