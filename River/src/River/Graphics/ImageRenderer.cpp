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
		layout (location = 1) in vec4 a_Color;
		layout (location = 2) in float a_TexSlot;
		layout (location = 3) in vec2 a_TexCoord;
		layout (location = 4) in float a_NumTextureChannels;

		uniform mat4 u_viewMatrix;

		out vec4 o_Color;
		out float o_TexSlot;
		out vec2 o_TexCoord;
		out float o_NumTextureChannels;

		void main()
		{
			gl_Position = u_viewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
			o_TexCoord = a_TexCoord;
			o_TexSlot = a_TexSlot;
			o_NumTextureChannels = a_NumTextureChannels;
			o_Color = a_Color;
		}
	)";

	static std::string fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
  
		in vec4 o_Color;
		in vec2 o_TexCoord;	
		in float o_TexSlot;
		in float o_NumTextureChannels;

		uniform sampler2D u_Textures[32];

		void main() {
			vec4 textureColor;
			if( o_TexSlot < 0 ) {
				textureColor = vec4(1.0);
			} else {
				textureColor = texture(u_Textures[int(o_TexSlot)], o_TexCoord);
			}

			if( int(o_NumTextureChannels) == 1 ){
				FragColor = o_Color;
				FragColor.a *= textureColor.r;
			}else{
				FragColor = textureColor * o_Color;
			}	
		}
	)";


	ImageRenderer::ImageRenderer(River::Window* window) :
		River::Renderer(window),
		textureBinder(window->getNumTextureSlots())
	{
		// Shader program
		Shader vertexShader(Shader::Type::VERTEX, vertexShaderSource);
		Shader fragmentShader(Shader::Type::FRAGMENT, fragmentShaderSource);

		shaderProgram = new ShaderProgram();
		shaderProgram->setVertexShader(&vertexShader);
		shaderProgram->setFragmentShader(&fragmentShader);
		shaderProgram->build();

		vertexArray.initialize();
	}


	void ImageRenderer::drawRectangle(const TransformData& transformData) {
		drawTexture({}, transformData);
	}

	void ImageRenderer::drawSprite(const Texture* sprite, const TransformData& transformData){
		ImageRenderer::TextureData textureData{ sprite->getImage(), sprite->getTextureCoordinates() };
		drawTexture(textureData, transformData);
	}


	void ImageRenderer::drawTexture(const ImageRenderer::TextureData& textureData, const ImageRenderer::TransformData& transformData) {

		int textureSlot = -1; // -1 is no texture (just color)
		unsigned int numTextureChannels = 0;
		if( textureData.texture != nullptr ) {
			try {
				textureSlot = textureBinder.addImage(textureData.texture);
			} catch( NoTextureSlotException e ) {
				flush();
				textureSlot = textureBinder.addImage(textureData.texture);
			}
			numTextureChannels = textureData.texture->getNumChannels();
		}
		
		unsigned int verticesOffset = vertexArray.getNumVertices();
		ImageVertex* vertices = vertexArray.nextVertices(4);

		// Variables for translation and rotation
		float halfWidth = transformData.width / 2.0f;
		float halfHeight = transformData.height / 2.0f;
		float sin = glm::sin(glm::radians(-transformData.rotation));
		float cos = glm::cos(glm::radians(-transformData.rotation));
		glm::mat3x2 transform = { { cos, sin }, { -sin, cos }, { transformData.x, transformData.y } };

		glm::vec2 position;

		// TODO: Fix correct color tinting

		position = transform * glm::vec3(-halfWidth, halfHeight, 1);
		vertices[0].x = position.x;
		vertices[0].y = position.y;
		vertices[0].z = transformData.z;
		vertices[0].r = transformData.color.r;
		vertices[0].g = transformData.color.g;
		vertices[0].b = transformData.color.b;
		vertices[0].a = transformData.color.a;
		vertices[0].textureSlot = (GLfloat) textureSlot;
		vertices[0].textureX = textureData.textureCoordinates.x1;
		vertices[0].textureY = textureData.textureCoordinates.y1;
		vertices[0].numTextureChannels = numTextureChannels;

		position = transform * glm::vec3(halfWidth, halfHeight, 1);
		vertices[1].x = position.x;
		vertices[1].y = position.y;
		vertices[1].z = transformData.z;
		vertices[1].r = transformData.color.r;
		vertices[1].g = transformData.color.g;
		vertices[1].b = transformData.color.b;
		vertices[1].a = transformData.color.a;
		vertices[1].textureSlot = (GLfloat)textureSlot;
		vertices[1].textureX = textureData.textureCoordinates.x2;
		vertices[1].textureY = textureData.textureCoordinates.y1;
		vertices[1].numTextureChannels = numTextureChannels;

		position = transform * glm::vec3(-halfWidth, -halfHeight, 1);
		vertices[2].x = position.x;
		vertices[2].y = position.y;
		vertices[2].z = transformData.z;
		vertices[2].r = transformData.color.r;
		vertices[2].g = transformData.color.g;
		vertices[2].b = transformData.color.b;
		vertices[2].a = transformData.color.a;
		vertices[2].textureSlot = (GLfloat)textureSlot;
		vertices[2].textureX = textureData.textureCoordinates.x1;
		vertices[2].textureY = textureData.textureCoordinates.y2;
		vertices[2].numTextureChannels = numTextureChannels;

		position = transform * glm::vec3(halfWidth, -halfHeight, 1);
		vertices[3].x = position.x;
		vertices[3].y = position.y;
		vertices[3].z = transformData.z;
		vertices[3].r = transformData.color.r;
		vertices[3].g = transformData.color.g;
		vertices[3].b = transformData.color.b;
		vertices[3].a = transformData.color.a;
		vertices[3].textureSlot = (GLfloat) textureSlot;
		vertices[3].textureX = textureData.textureCoordinates.x2;
		vertices[3].textureY = textureData.textureCoordinates.y2;
		vertices[3].numTextureChannels = numTextureChannels;

		unsigned int* indices = vertexArray.nextIndices(6);
		indices[0] = verticesOffset + 0;
		indices[1] = verticesOffset + 1;
		indices[2] = verticesOffset + 2;
		indices[3] = verticesOffset + 1;
		indices[4] = verticesOffset + 2;
		indices[5] = verticesOffset + 3;
	}


	void ImageRenderer::onFlush(){

		// Enable alpha testing, and discarding any fragment, which has an alpha of 0
		GL(glEnable(GL_ALPHA_TEST));
		GL(glAlphaFunc(GL_GREATER, 0));
		GL(glEnable(GL_DEPTH_TEST));
		
		if( blending ) {
			// We expect things to be drawn in order when blending
			GL(glEnable(GL_BLEND));
			GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			GL(glDepthMask(GL_FALSE)); // Disable writing to depth buffer 
		} else {
			GL(glDisable(GL_BLEND));
			GL(glDepthMask(GL_TRUE)); // Enable writing to depth buffer
		}

		shaderProgram->use();
		textureBinder.bind(shaderProgram);
		shaderProgram->setFloatMatrix("u_viewMatrix", 4, glm::value_ptr(camera->getCameraMatrix()));
		vertexArray.bind();
		GL(glDrawElements(GL_TRIANGLES, vertexArray.getNumIndices(), GL_UNSIGNED_INT, 0));
		vertexArray.unbind();
		vertexArray.clear();

		textureBinder.clear();
	}



	void ImageRenderer::enableBlending() {
		blending = true;
	}


	void ImageRenderer::disableBlending() {
		blending = false;
	}
}



