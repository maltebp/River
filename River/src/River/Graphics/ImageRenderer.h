#pragma once

#include "Renderer.h"
#include "Texture.h"
#include "Window.h"
#include "Shader/ShaderProgram.h"
#include "VertexArray.h"


namespace River {

	// Vertex Array ------------------------------------------
	struct ImageVertex {
		GLfloat x, y, z;
		GLfloat textureSlot;
		GLfloat textureX, textureY;
	};

	class ImageVertexArray : public VertexArray<ImageVertex> {
	protected:
		void setupAttributes() override {
			addAttributeFloat(3); // Position
			addAttributeFloat(1);  // Texture slot
			addAttributeFloat(2); // Tex position
		}
	};


	// ImageRenderer -----------------------------------------
	class ImageRenderer : public Renderer {

	private:
		ImageVertexArray vertexArray;

		ShaderProgram* shaderProgram;
		TextureBinder textureBinder;

	public:

		ImageRenderer(Window *window);

		void drawImage(Texture *texture, float x, float y, float z, float width, float height, float rotation);

		void onFlush();

		// TODO: Function -> Draw rotated texture

		// TODO: Function -> draw texture with texture size
	};



	
}



