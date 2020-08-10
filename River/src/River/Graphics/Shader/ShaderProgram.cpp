#include "ShaderProgram.h"

#include "River/Graphics/GL.h"

River::ShaderProgram::ShaderProgram() {
	id = GL(glCreateProgram());
}

River::ShaderProgram::~ShaderProgram() {
	GL(glDeleteProgram(id));
}


void River::ShaderProgram::setVertexShader(River::Shader* shader) {
	if (!shader->isReady())
		throw new River::ShaderException("Fragment shader has not been compiled");
	vertexShader = shader;
}


void River::ShaderProgram::setFragmentShader(River::Shader* shader) {
	if (!shader->isReady())
		throw new River::ShaderException("Fragment shader has not been compiled");
	fragmentShader = shader;
}

void River::ShaderProgram::build() {

	// Check shaders
	if (vertexShader == nullptr) {
		throw new ShaderException("Shader program is missing vertex shader");
	}
	if (fragmentShader == nullptr) {
		throw new ShaderException("Shader program is missing vertex shader");
	}
	GL(glAttachShader(id, vertexShader->getId()));
	GL(glAttachShader(id, fragmentShader->getId()));

	// Link Program
	GL(glLinkProgram(id));
	int linkStatus;
	GL(glGetProgramiv(id, GL_LINK_STATUS, &linkStatus));
	if (linkStatus != GL_TRUE) {
		std::string statusMessage = getInfoLog();
		GL(glDetachShader(id, vertexShader->getId()));
		GL(glDetachShader(id, fragmentShader->getId()));
		throw River::ShaderException("Program failed linking (" + statusMessage + ")");
	}
	
	// Validate program
	GL(glValidateProgram(id));
	int validationStatus;
	GL(glGetProgramiv(id, GL_VALIDATE_STATUS, &validationStatus));
	if (validationStatus != GL_TRUE) {
		std::string statusMessage = getInfoLog();
		GL(glDetachShader(id, vertexShader->getId()));
		GL(glDetachShader(id, fragmentShader->getId()));
		throw River::ShaderException("Program failed linking (" + statusMessage + ")");
	}

	ready = true;
}


void River::ShaderProgram::use() {
	if ( !ready )
		throw new ShaderException("Shader program is not ready to be used"); 
	GL(glUseProgram(id));
}



std::string River::ShaderProgram::getInfoLog() {
		int messageLength;
		GL(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &messageLength));
		char* message = (char*) malloc(sizeof(char) * messageLength);

		GL(glGetProgramInfoLog(id, messageLength, NULL, message));
		std::string stringMessage = message;
		free(message);

		return stringMessage;
}