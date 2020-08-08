#include "ImageRenderer.h"

#include "Graphics.h"

#include "Shader/Shader.h"
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

	// Vertex buffer must be nound before settings attributes (apparently)
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (2*sizeof(float)) );

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Index buffer
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	// Shader program
	Shader vertexShader(Shader::Type::VERTEX, vertexShaderSource);
	Shader fragmentShader(Shader::Type::FRAGMENT, fragmentShaderSource);

	shaderProgram = new ShaderProgram();
	shaderProgram->setVertexShader(&vertexShader);
	shaderProgram->setFragmentShader(&fragmentShader);
	shaderProgram->build();
}


void River::ImageRenderer::drawImage(Texture *texture, float x, float y, float width, float height) {

	shaderProgram->use();

	glBindTexture(GL_TEXTURE_2D, texture->getId());

	float vertices[] = {
		x,			y + height,		0.0f, 1.0f,	// top left
		x + width,	y + height,		1.0f, 1.0f, // top right
		x,			y,				0.0f, 0.0f,	// bottom left
		x + width,	y,				1.0f, 0.0f	// bottom right
	};

	int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void River::ImageRenderer::flush() {

}


