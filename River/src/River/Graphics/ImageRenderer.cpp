#include "ImageRenderer.h"

#include "GL.h"

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


River::Texture* texture;

River::ImageRenderer::ImageRenderer(River::Window *window) : River::Renderer(window) {
	// Shader program
	Shader vertexShader(Shader::Type::VERTEX, vertexShaderSource);
	Shader fragmentShader(Shader::Type::FRAGMENT, fragmentShaderSource);

	shaderProgram = new ShaderProgram();
	shaderProgram->setVertexShader(&vertexShader);
	shaderProgram->setFragmentShader(&fragmentShader);
	shaderProgram->build();

	vertexArray.initialize();
}


void River::ImageRenderer::drawImage(River::Texture *tex, float x, float y, float width, float height) {

	shaderProgram->use();
	texture = tex;

	unsigned int verticesOffset = vertexArray.getNumVertices();
	ImageVertex* vertices = vertexArray.nextVertices(4);

	vertices[0].x = x;
	vertices[0].y = y + height;
	vertices[0].texX = 0.0f;
	vertices[0].texY = 1.0f;

	vertices[1].x = x + width;
	vertices[1].y = y + height;
	vertices[1].texX = 1.0f;
	vertices[1].texY = 1.0f;

	vertices[2].x = x;
	vertices[2].y = y;
	vertices[2].texX = 0.0f;
	vertices[2].texY = 0.0f;
	
	vertices[3].x = x + width;
	vertices[3].y = y;
	vertices[3].texX = 1.0f;
	vertices[3].texY = 0.0f;

	unsigned int* indices = vertexArray.nextIndices(6);
	indices[0] = verticesOffset + 0;
	indices[1] = verticesOffset + 1;
	indices[2] = verticesOffset + 2;
	indices[3] = verticesOffset + 1;
	indices[4] = verticesOffset + 2;
	indices[5] = verticesOffset + 3;

}


void River::ImageRenderer::flush() {
	GL(glBindTexture(GL_TEXTURE_2D, texture->getId()));
	vertexArray.bind();
	GL(glDrawElements(GL_TRIANGLES, vertexArray.getNumIndices(), GL_UNSIGNED_INT, 0));
	vertexArray.unbind();
	vertexArray.clear();
}



