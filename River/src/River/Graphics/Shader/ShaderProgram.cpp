#include "ShaderProgram.h"



namespace River{

	ShaderProgram::ShaderProgram(){
		id = GL(glCreateProgram());
	}

	ShaderProgram::~ShaderProgram(){
		GL(glDeleteProgram(id));
	}


	void ShaderProgram::setVertexShader(Shader* shader){
		if( !shader->isReady() )
			throw new ShaderException("Fragment shader has not been compiled");
		vertexShader = shader;
	}


	void ShaderProgram::setFragmentShader(Shader* shader){
		if( !shader->isReady() )
			throw new ShaderException("Fragment shader has not been compiled");
		fragmentShader = shader;
	}

	void ShaderProgram::build(){

		// Check shaders
		if( vertexShader == nullptr ){
			throw new ShaderException("Shader program is missing vertex shader");
		}
		if( fragmentShader == nullptr ){
			throw new ShaderException("Shader program is missing vertex shader");
		}
		GL(glAttachShader(id, vertexShader->getId()));
		GL(glAttachShader(id, fragmentShader->getId()));

		// Link Program
		GL(glLinkProgram(id));
		int linkStatus;
		GL(glGetProgramiv(id, GL_LINK_STATUS, &linkStatus));
		if( linkStatus != GL_TRUE ){
			std::string statusMessage = getInfoLog();
			GL(glDetachShader(id, vertexShader->getId()));
			GL(glDetachShader(id, fragmentShader->getId()));
			throw ShaderException("Program failed linking (" + statusMessage + ")");
		}

		// Validate program
		GL(glValidateProgram(id));
		int validationStatus;
		GL(glGetProgramiv(id, GL_VALIDATE_STATUS, &validationStatus));
		if( validationStatus != GL_TRUE ){
			std::string statusMessage = getInfoLog();
			GL(glDetachShader(id, vertexShader->getId()));
			GL(glDetachShader(id, fragmentShader->getId()));
			throw ShaderException("Program failed linking (" + statusMessage + ")");
		}

		ready = true;
	}


	void ShaderProgram::use(){
		if( !ready )
			throw new ShaderException("Shader program is not ready to be used");
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


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Uniforms

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

}
