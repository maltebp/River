#include "ShaderProgram.h"

#include "River/Graphics/Graphics.h"

River::ShaderProgram::ShaderProgram() {
	id = glCreateProgram();
}

River::ShaderProgram::~ShaderProgram() {
	glDeleteProgram(id);
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
	glAttachShader(id, vertexShader->getId());
	glAttachShader(id, fragmentShader->getId());

	// Link Program
	glLinkProgram(id);
	int linkStatus;
	glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		std::string statusMessage = getInfoLog();
		glDetachShader(id, vertexShader->getId());
		glDetachShader(id, fragmentShader->getId());
		throw River::ShaderException("Program failed linking (" + statusMessage + ")");
	}
	
	// Validate program
	glValidateProgram(id);
	int validationStatus;
	glGetProgramiv(id, GL_VALIDATE_STATUS, &validationStatus);
	if (validationStatus != GL_TRUE) {
		std::string statusMessage = getInfoLog();
		glDetachShader(id, vertexShader->getId());
		glDetachShader(id, fragmentShader->getId());
		throw River::ShaderException("Program failed linking (" + statusMessage + ")");
	}

	ready = true;
}


void River::ShaderProgram::use() {
	if ( !ready )
		throw new ShaderException("Shader program is not ready to be used"); 
	glUseProgram(id);
}



std::string River::ShaderProgram::getInfoLog() {
		int messageLength;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &messageLength);
		char* message = (char*) malloc(sizeof(char) * messageLength);

		glGetProgramInfoLog(id, messageLength, NULL, message);
		std::string stringMessage = message;
		free(message);

		return stringMessage;
}