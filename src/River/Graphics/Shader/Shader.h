#pragma once

namespace River {

	class Shader {
	public:

		/**
		 * @param type		The OpenGL shader type (i.e. GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
		 * @param source	The source code for the shader
		 * 
		 * @throws River::ShaderException Thrown if someone goes wrong when loading the shader (i.e. source code couldn't compile)
		*/
		Shader(unsigned int typeId, const std::string& source);

		// Pure virtual to ensure this class is abstract
		virtual ~Shader() = 0;

		unsigned int getId() const;

		bool isReady() const;
		

	private:
		
		unsigned int id = 0;
		
		std::string source;
		
		bool ready = false;

	};

}

