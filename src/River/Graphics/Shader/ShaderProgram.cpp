#include "River/pch.h"

#include "ShaderProgram.h"



namespace River{

	ShaderProgram::ShaderProgram() {

	}


	ShaderProgram::~ShaderProgram() {
		if( glIsProgram(id) ) {
			GL(glDeleteProgram(id));
		}
	}


	void ShaderProgram::build(const VertexShader& vertexShader, const FragmentShader& fragmentShader ){
		if( ready ) {
			throw new InvalidStateException("Shader has already been loaded");
		}

		if( !vertexShader.isReady() ){
			throw new ShaderException("Vertex shader is not ready");
		}

		if( !fragmentShader.isReady() ){
			throw new ShaderException("Fragment shader is not ready");
		}

		id = GL(glCreateProgram());

		GL(glAttachShader(id, vertexShader.getId()));
		GL(glAttachShader(id, fragmentShader.getId()));

		// Link Program
		GL(glLinkProgram(id));
		int linkStatus;
		GL(glGetProgramiv(id, GL_LINK_STATUS, &linkStatus));
		if( linkStatus != GL_TRUE ){
			std::string statusMessage = getInfoLog();
			GL(glDetachShader(id, vertexShader.getId()));
			GL(glDetachShader(id, fragmentShader.getId()));
			throw ShaderException("Program failed linking (" + statusMessage + ")");
		}

		// Validate program
		GL(glValidateProgram(id));
		int validationStatus;
		GL(glGetProgramiv(id, GL_VALIDATE_STATUS, &validationStatus));
		if( validationStatus != GL_TRUE ){
			std::string statusMessage = getInfoLog();
			GL(glDetachShader(id, vertexShader.getId()));
			GL(glDetachShader(id, fragmentShader.getId()));
			throw ShaderException("Program failed linking (" + statusMessage + ")");
		}

		ready = true;
	}


	void ShaderProgram::use(){
		if( !ready ) {
			throw new ShaderException("ShaderProgram has not been built");
		}
		GL(glUseProgram(id));
	}



	std::string ShaderProgram::getInfoLog(){
		int messageLength;
		GL(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &messageLength));
		char* message = (char*)malloc(sizeof(char) * messageLength);

		GL(glGetProgramInfoLog(id, messageLength, NULL, message));
		std::string stringMessage = message;
		free(message);

		return stringMessage;
	}


	void ShaderProgram::setIntArray(const std::string &name, int* values, int count){
		GLint location = getUniformLocation(name);
		GL(glUniform1iv(location, count, values));
	}


	void ShaderProgram::setFloatMatrix(const std::string &name, int size, float *matrix){
		auto location = getUniformLocation(name);
		switch( size ){
		case 4:
			GL(glUniformMatrix4fv(location, 1, GL_FALSE, matrix));
		}
	}


	bool ShaderProgram::hasUniform(const std::string& name){
		GLint location = GL(glGetUniformLocation(id, name.c_str()));
		return location != -1;
	}


	GLint ShaderProgram::getUniformLocation(const std::string &name){
		GLint location = GL(glGetUniformLocation(id, name.c_str()));
		if( location == -1 )
			throw new ShaderException("Uniform '" + name + "' doesn't exist in program");
		return location;
	}


	bool ShaderProgram::isReady() {
		return ready;
	}

}
