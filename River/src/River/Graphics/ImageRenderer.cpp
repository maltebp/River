#include "ImageRenderer.h"

#include <iostream>

#include <River/Vendor/glm/gtc/matrix_transform.hpp>
#include <River/Vendor/glm/glm.hpp>

#include "GL.h"
#include "Shader/Shader.h"
#include "River/Error.h"

namespace River{
	static std::string vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in float a_TexSlot;
		layout (location = 2) in vec2 a_TexCoord;
		layout (location = 3) in float a_NumTextureChannels;

		uniform mat4 u_viewMatrix;

		out float o_TexSlot;
		out vec2 o_TexCoord;
		out float o_NumTextureChannels;

		void main()
		{
			gl_Position = u_viewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
			o_TexCoord = a_TexCoord;
			o_TexSlot = a_TexSlot;
			o_NumTextureChannels = a_NumTextureChannels;
		}
	)";

	static std::string fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
  
		in vec2 o_TexCoord;	
		in float o_TexSlot;
		in float o_NumTextureChannels;

		uniform sampler2D u_Textures[32];

		void main() {
			int index = int(o_TexSlot);
			vec4 color = texture(u_Textures[index], o_TexCoord);

			if( int(o_NumTextureChannels) == 1 )
				FragColor = vec4(1.0, 1.0, 1.0, color.r);
			else
				FragColor = color;

		}
	)";

	/*
	
		
	*/




	ImageRenderer::ImageRenderer(River::Window* window) :
		River::Renderer(window),
		textureBinder(window->getNumTextureSlots(), true)
	{
		// Shader program
		Shader vertexShader(Shader::Type::VERTEX, vertexShaderSource);
		Shader fragmentShader(Shader::Type::FRAGMENT, fragmentShaderSource);

		shaderProgram = new ShaderProgram();
		shaderProgram->setVertexShader(&vertexShader);
		shaderProgram->setFragmentShader(&fragmentShader);
		shaderProgram->build();

		vertexArray.initialize();

		std::cout << "Size of struct: " << sizeof(ImageVertex) << std::endl;
	}


	void ImageRenderer::drawSprite(const Sprite* sprite, float x, float y, float z, float width, float height, float rotation){
		
		Texture *texture = sprite->getTexture();
		auto &textureCoordinates = sprite->getTextureCoordinates();

		unsigned int textureSlot;
		try{
			textureSlot = textureBinder.addTexture(texture);
		} catch(NoTextureSlotException e){
			flush();
			textureSlot = textureBinder.addTexture(texture);
		}

		unsigned int verticesOffset = vertexArray.getNumVertices();
		ImageVertex* vertices = vertexArray.nextVertices(4);

		// Variables for translation and rotation
		float halfWidth = width / 2.0f;
		float halfHeight = height / 2.0f;
		float sin = glm::sin(glm::radians(-rotation));
		float cos = glm::cos(glm::radians(-rotation));
		glm::mat3x2 transform = {{ cos, sin }, { -sin, cos }, { x, y }};

		glm::vec2 position;

		position = transform * glm::vec3(-halfWidth, halfHeight, 1);
		vertices[0].x = position.x;
		vertices[0].y = position.y;
		vertices[0].z = z;
		vertices[0].textureSlot = (GLfloat) textureSlot;
		vertices[0].textureX = textureCoordinates.x1;
		vertices[0].textureY = textureCoordinates.y1;
		vertices[0].numTextureChannels = texture->getNumChannels();

		position = transform * glm::vec3(halfWidth, halfHeight, 1);
		vertices[1].x = position.x;
		vertices[1].y = position.y;
		vertices[1].z = z;
		vertices[1].textureSlot = (GLfloat) textureSlot;
		vertices[1].textureX = textureCoordinates.x2;
		vertices[1].textureY = textureCoordinates.y1;
		vertices[1].numTextureChannels = texture->getNumChannels();

		position = transform * glm::vec3(-halfWidth, -halfHeight, 1);
		vertices[2].x = position.x;
		vertices[2].y = position.y;
		vertices[2].z = z;
		vertices[2].textureSlot = (GLfloat) textureSlot;
		vertices[2].textureX = textureCoordinates.x1;
		vertices[2].textureY = textureCoordinates.y2;
		vertices[2].numTextureChannels = texture->getNumChannels();

		position = transform * glm::vec3(halfWidth, -halfHeight, 1);
		vertices[3].x = position.x;
		vertices[3].y = position.y;
		vertices[3].z = z;
		vertices[3].textureSlot = (GLfloat) textureSlot;
		vertices[3].textureX = textureCoordinates.x2;
		vertices[3].textureY = textureCoordinates.y2;
		vertices[3].numTextureChannels = texture->getNumChannels();


		unsigned int* indices = vertexArray.nextIndices(6);
		indices[0] = verticesOffset + 0;
		indices[1] = verticesOffset + 1;
		indices[2] = verticesOffset + 2;
		indices[3] = verticesOffset + 1;
		indices[4] = verticesOffset + 2;
		indices[5] = verticesOffset + 3;

	}


	void ImageRenderer::onFlush(){
		shaderProgram->use();
		textureBinder.bind(shaderProgram);
		shaderProgram->setFloatMatrix("u_viewMatrix", 4, glm::value_ptr(camera->getCameraMatrix()) );
		vertexArray.bind();
		GL(glDrawElements(GL_TRIANGLES, vertexArray.getNumIndices(), GL_UNSIGNED_INT, 0));
		vertexArray.unbind();
		vertexArray.clear();

		textureBinder.clear();
	}

}



