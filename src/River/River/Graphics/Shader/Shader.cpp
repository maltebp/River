#include "River/pch.h"

#include "Shader.h"

#include "River/Graphics/GL.h"
#include "River/Error.h"


namespace River {


	Shader::Shader(unsigned int typeId, const std::string& source) {

		this->source = source;

		if( typeId != GL_VERTEX_SHADER && typeId != GL_FRAGMENT_SHADER ) {
			throw InvalidArgumentException("Shader type is invalid");
		}

		id = GL(glCreateShader(typeId));

		const char* cSource = source.c_str();
		GL(glShaderSource(id, 1, &cSource, nullptr));

		// Compile shader
		GL(glCompileShader(id));
		int compileStatus;	
		GL(glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus));
		
		if( compileStatus != GL_TRUE ) {
			int messageLength;
			GL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength));
			GLchar* message = (GLchar*)malloc(sizeof(GLchar) * (messageLength));
			GL(glGetShaderInfoLog(id, messageLength, NULL, message));
			std::string stringMessage = message;
			free(message);
			GL(glDeleteShader(id));
			throw River::ShaderException("Shader compilation failed with the message '" + stringMessage + "'");
		}

		ready = true;
	}


	Shader::~Shader() {
		// glIsShader does not throw error
		if( glIsShader(id) == GL_TRUE ) { 
			GL(glDeleteShader(id));
		}
	}


	unsigned int Shader::getId() const {
		return id;
	}


	bool Shader::isReady() const {
		return ready;
	}

}




