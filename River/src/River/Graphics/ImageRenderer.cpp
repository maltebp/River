#include "ImageRenderer.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "River/Error.h"

std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

std::string fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}
)";

//




River::ImageRenderer::ImageRenderer(River::Window *window) : River::Renderer(window) {
	

	// Setup data layout
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (2*sizeof(float)) );

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	// Shader program
	Shader vertexShader(GL_VERTEX_SHADER, vertexShaderSource);
	Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader.getId());
	glAttachShader(shaderProgram, fragmentShader.getId());

	glLinkProgram(shaderProgram);
	int linkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
	if( linkStatus != GL_TRUE ){
		// TODO: Print program info log
		throw River::ShaderException("Program could not be linked");
	}

	glValidateProgram(shaderProgram);
	int validationStatus;
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &validationStatus);
	if (validationStatus != GL_TRUE) {
		// TODO: Print program info log
		throw River::ShaderException("Program could not be validated");
	}

	// TODO: Clean up if program build fails

}


void River::ImageRenderer::drawImage(Texture *texture, float x, float y, float width, float height) {

	glUseProgram(shaderProgram);

	glBindTexture(GL_TEXTURE_2D, texture->getId());

	float vertices[] = {
		x + width,	y + height,		1.0f, 1.0f, // top right
		x + width,	y,				1.0f, 0.0f,	// bottom right
		x,			y,				0.0f, 0.0f,	// bottom left
		x + width,	y + height,		1.0f, 1.0f, // top right
		x,			y,				0.0f, 0.0f,	// bottom left
		x,			y + height,		0.0f, 1.0f,	// top left
	};

	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW );

	glDrawArrays(GL_TRIANGLES, 0, 6);
}


void River::ImageRenderer::flush() {

}


