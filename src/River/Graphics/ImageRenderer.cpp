#include "River/pch.h"

#include "ImageRenderer.h"

#include <River/External/glm/gtc/matrix_transform.hpp>
#include <River/External/glm/glm.hpp>

#include "GL.h"
#include "Shader/Shader.h"
#include "River/Error.h"


namespace River{

	static std::string vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec4 a_Color;
		layout (location = 2) in int a_TexSlot;
		layout (location = 3) in vec2 a_TexCoord;
		layout (location = 4) in int a_NumTextureChannels;

		uniform mat4 u_viewMatrix;

		out vec4 o_Color;
		flat out int o_TexSlot;
		out vec2 o_TexCoord;
		flat out int o_NumTextureChannels;

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
		flat in int o_TexSlot;
		flat in int o_NumTextureChannels;

		uniform sampler2D u_Textures[32];

		void main() {
			vec4 textureColor;
			if( o_TexSlot < 0 ) {
				textureColor = vec4(1.0);
			} else {
				textureColor = texture(u_Textures[o_TexSlot], o_TexCoord);
			}

			if( o_NumTextureChannels == 1 ){
				FragColor = o_Color;
				FragColor.a *= textureColor.r;
			}else{
				FragColor = textureColor * o_Color;
			}	
		}
	)";


	ImageRenderer::ImageRenderer()
		:	textureBinder(Window::getNumTextureSlots())
	{
		// Shader program
		VertexShader vertexShader(vertexShaderSource);
		FragmentShader fragmentShader(fragmentShaderSource);

		shaderProgram.build(vertexShader, fragmentShader);

		vertexSize = vertexArray.setAttributes( &vertexBuffer,
			VertexAttributes::FLOAT3,	// Position
			VertexAttributes::FLOAT4,	// Color
			VertexAttributes::BYTE, 	// Texture slot
			VertexAttributes::FLOAT2,	// Texture coordinates
			VertexAttributes::BYTE		// Texture channels
		);

		vertexArray.setIndexBuffer(&indexBuffer, VertexIndexTypes::UINT);
	}

	
	void ImageRenderer::setCamera(Camera* camera) {
		this->camera = camera;
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
			
			textureSlot = textureBinder.addImage(textureData.texture);

			if( textureSlot < 0 ) {
				flush();
				textureSlot = textureBinder.addImage(textureData.texture);
			}

			numTextureChannels = textureData.texture->getNumChannels();
		}
		
		// Reserve memory for 4 vertices
		vertexData.reserveExtra(vertexSize * 4);

		// Variables for translation and rotation
		float halfWidth = transformData.width / 2.0f;
		float halfHeight = transformData.height / 2.0f;
		float sin = glm::sin(glm::radians(transformData.rotation));
		float cos = glm::cos(glm::radians(transformData.rotation));
		glm::mat3x2 transform = { { cos, sin }, { -sin, cos }, { transformData.x, transformData.y } };

		glm::vec2 position;

		// TODO: Fix correct color tinting

		position = transform * glm::vec3(-halfWidth, halfHeight, 1);
		vertexData.add((GLfloat)position.x);
		vertexData.add((GLfloat)position.y);
		vertexData.add((GLfloat)transformData.z);
		vertexData.add((GLfloat)transformData.color.r);
		vertexData.add((GLfloat)transformData.color.g);
		vertexData.add((GLfloat)transformData.color.b);
		vertexData.add((GLfloat)transformData.color.a * transformData.opacity);
		vertexData.add((GLbyte)textureSlot);
		vertexData.add((GLfloat)textureData.textureCoordinates.x1);
		vertexData.add((GLfloat)textureData.textureCoordinates.y1);
		vertexData.add((GLbyte)numTextureChannels);

		position = transform * glm::vec3(halfWidth, halfHeight, 1);
		vertexData.add((GLfloat)position.x);
		vertexData.add((GLfloat)position.y);
		vertexData.add((GLfloat)transformData.z);
		vertexData.add((GLfloat)transformData.color.r);
		vertexData.add((GLfloat)transformData.color.g);
		vertexData.add((GLfloat)transformData.color.b);
		vertexData.add((GLfloat)transformData.color.a * transformData.opacity);
		vertexData.add((GLbyte)textureSlot);
		vertexData.add((GLfloat)textureData.textureCoordinates.x2);
		vertexData.add((GLfloat)textureData.textureCoordinates.y1);
		vertexData.add((GLbyte)numTextureChannels);

		position = transform * glm::vec3(-halfWidth, -halfHeight, 1);
		vertexData.add((GLfloat)position.x);
		vertexData.add((GLfloat)position.y);
		vertexData.add((GLfloat)transformData.z);
		vertexData.add((GLfloat)transformData.color.r);
		vertexData.add((GLfloat)transformData.color.g);
		vertexData.add((GLfloat)transformData.color.b);
		vertexData.add((GLfloat)transformData.color.a * transformData.opacity);
		vertexData.add((GLbyte)textureSlot);
		vertexData.add((GLfloat)textureData.textureCoordinates.x1);
		vertexData.add((GLfloat)textureData.textureCoordinates.y2);
		vertexData.add((GLbyte)numTextureChannels);

		position = transform * glm::vec3(halfWidth, -halfHeight, 1);
		vertexData.add((GLfloat)position.x);
		vertexData.add((GLfloat)position.y);
		vertexData.add((GLfloat)transformData.z);
		vertexData.add((GLfloat)transformData.color.r);
		vertexData.add((GLfloat)transformData.color.g);
		vertexData.add((GLfloat)transformData.color.b);
		vertexData.add((GLfloat)transformData.color.a * transformData.opacity);
		vertexData.add((GLbyte)textureSlot);
		vertexData.add((GLfloat)textureData.textureCoordinates.x2);
		vertexData.add((GLfloat)textureData.textureCoordinates.y2);
		vertexData.add((GLbyte)numTextureChannels);

		GLuint verticesOffset = numTextures * 4;
		indexData.push_back(verticesOffset + 0);
		indexData.push_back(verticesOffset + 1);
		indexData.push_back(verticesOffset + 2);
		indexData.push_back(verticesOffset + 1);
		indexData.push_back(verticesOffset + 2);
		indexData.push_back(verticesOffset + 3);

		numTextures++;
	}


	void ImageRenderer::flush(){

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

		shaderProgram.use();
		textureBinder.bind(&shaderProgram, "u_Textures");
		shaderProgram.setFloatMatrix("u_viewMatrix", 4, glm::value_ptr(camera->getCameraMatrix()));

		vertexBuffer.setData(vertexData.getVector());
		indexBuffer.setData(indexData);

		vertexArray.drawTriangles(numTextures * 2);

		vertexData.clear();
		indexData.clear();
		textureBinder.clear();

		numTextures = 0;
	}



	void ImageRenderer::enableBlending() {
		blending = true;
	}


	void ImageRenderer::disableBlending() {
		blending = false;
	}
}



