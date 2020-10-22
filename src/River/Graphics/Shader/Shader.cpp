#include "Shader.h"

#include "River/Graphics/GL.h"

#include "River/Error.h"

River::Shader::Shader(Type type, const std::string& source) {
	this->type = type;
	this->source = source;
	
	if( type == Type::VERTEX )
		id = GL(glCreateShader(GL_VERTEX_SHADER));
	if (type == Type::FRAGMENT)
		id = GL(glCreateShader(GL_FRAGMENT_SHADER));

	const char* cSource = source.c_str();
	GL(glShaderSource(id, 1, &cSource, nullptr ));

	// Compile shader
	GL(glCompileShader(id));
	int compileStatus;
	GL(glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus));
	if (compileStatus != GL_TRUE) {
		int messageLength;
		GL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength));
		GLchar* message = (GLchar*) malloc(sizeof(GLchar) * (messageLength));
		GL(glGetShaderInfoLog(id, messageLength, NULL, message));
		std::string stringMessage = message;
		free(message);
		GL(glDeleteShader(id));		throw River::ShaderException("Shader compilation failed with the message '" + stringMessage + "'");
	}

	ready = true;
}

River::Shader::~Shader() {
	if(glIsShader(id) == GL_TRUE) { // glIsShader doesn't give error
		GL(glDeleteShader(id));
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