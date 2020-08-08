#include "Shader.h"

#include "Graphics.h"

#include "River/Error.h"

River::Shader::Shader(unsigned int type, const std::string& source) {
	
	this->source = source;
	
	id = glCreateShader(type);
	const char* cSource = source.c_str();
	glShaderSource(id, 1, &cSource, nullptr );

	// Compile shader
	glCompileShader(id);
	int compileStatus;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		int messageLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength);
		char* message = (char*) malloc(sizeof(char) * messageLength);
		glGetShaderInfoLog(id, messageLength, NULL, message);
		std::string stringMessage = message;
		free(message);
		glDeleteShader(id);
		throw River::ShaderException("Shader compilation failed with the message '" + stringMessage + "'");
	}

}

River::Shader::~Shader() {
	if (glIsShader(id) == GL_TRUE) {
		glDeleteShader(id);
	}
}

unsigned int River::Shader::getId() {
	return id;
}