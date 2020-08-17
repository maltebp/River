#include "ImageRenderer.h"

#include "GL.h"

#include "Shader/Shader.h"
#include "River/Error.h"

namespace River{
	static std::string vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in float a_TexSlot;
		layout (location = 2) in vec2 a_TexCoord;

		uniform mat4 u_viewMatrix;

		out float o_TexSlot;
		out vec2 o_TexCoord;

		void main()
		{
			gl_Position = u_viewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
			o_TexCoord = a_TexCoord;
			o_TexSlot = a_TexSlot;
		}
	)";

	static std::string fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
  
		in vec2 o_TexCoord;
		in float o_TexSlot;

		uniform sampler2D u_Textures[32];

		void main()
		{
			int index = int(o_TexSlot);
			FragColor = texture(u_Textures[index], o_TexCoord);
		}
	)";


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


	void ImageRenderer::drawImage(River::Texture* texture, float x, float y, float z, float width, float height){
		
		unsigned int textureSlot;
		try{
			textureSlot = textureBinder.addTexture(texture);
		} catch(NoTextureSlotException e){
			flush();
			textureSlot = textureBinder.addTexture(texture);
		}

		unsigned int verticesOffset = vertexArray.getNumVertices();
		ImageVertex* vertices = vertexArray.nextVertices(4);

		vertices[0].x = x;
		vertices[0].y = y + height;
		vertices[0].z = z;
		vertices[0].textureSlot = (GLfloat) textureSlot;
		vertices[0].textureX = 0.0f;
		vertices[0].textureY = 1.0f;


		vertices[1].x = x + width;
		vertices[1].y = y + height;
		vertices[1].z = z;
		vertices[1].textureSlot = (GLfloat) textureSlot;
		vertices[1].textureX = 1.0f;
		vertices[1].textureY = 1.0f;

		vertices[2].x = x;
		vertices[2].y = y;
		vertices[2].z = z;
		vertices[2].textureSlot = (GLfloat) textureSlot;
		vertices[2].textureX = 0.0f;
		vertices[2].textureY = 0.0f;

		vertices[3].x = x + width;
		vertices[3].y = y;
		vertices[3].z = z;
		vertices[3].textureSlot = (GLfloat) textureSlot;
		vertices[3].textureX = 1.0f;
		vertices[3].textureY = 0.0f;

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
		shaderProgram->setFloatMatrix("u_viewMatrix", 4, glm::value_ptr(camera->getViewMatrix()) );
		vertexArray.bind();
		GL(glDrawElements(GL_TRIANGLES, vertexArray.getNumIndices(), GL_UNSIGNED_INT, 0));
		vertexArray.unbind();
		vertexArray.clear();

		textureBinder.clear();
	}

}



