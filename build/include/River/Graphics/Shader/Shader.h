#pragma once

#include <string>

namespace River {

	class Shader {

	public:
		enum class Type {
			VERTEX,
			FRAGMENT
		};
		
	private:
		unsigned int id = 0;
		Type type;
		std::string source;
		bool ready = false;

	public:

		/**
		 * @param type The OpenGL shader type (i.e. GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
		 * @param source The source code for the shader
		 * @throws River::ShaderException Thrown if someone goes wrong when loading the shader (i.e. source code couldn't compile)
		*/
		Shader(Type type, const std::string &source);
		~Shader();

		unsigned int getId();

		bool isReady();

		Type getType();

	};

}

