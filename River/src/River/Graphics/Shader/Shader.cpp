#include "Shader.h"

#include "River/Graphics/Graphics.h"

#include "River/Error.h"

River::Shader::Shader(Type type, const std::string& source) {
	this->type = type;
	this->source = source;
	
	if( type == Type::VERTEX )
		id = glCreateShader(GL_VERTEX_SHADER);
	if (type == Type::FRAGMENT)
		id = glCreateShader(GL_FRAGMENT_SHADER);

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

	ready = true;
}

River::Shader::~Shader() {
	if (glIsShader(id) == GL_TRUE) {
		glDeleteShader(id);
	}
}

unsigned int River::Shader::getId() {
	return id;
}

bool River::Shader::isReady() {
	return ready;
}

River::Shader::Type River::Shader::getType() {
	return type;
}