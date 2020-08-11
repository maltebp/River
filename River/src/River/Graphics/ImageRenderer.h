#pragma once

#include "Renderer.h"
#include "Texture.h"
#include "Window.h"
#include "Shader/ShaderProgram.h"
#include "VertexArray.h"


namespace River {

	// Vertex Array ------------------------------------------
	struct ImageVertex {
		GLfloat x, y;
		GLfloat textureSlot;
		GLfloat textureX, textureY;
	};

	class ImageVertexArray : public VertexArray<ImageVertex> {
	protected:
		void setupAttributes() override {
			addAttributeFloat(2); // Position
			addAttributeFloat(1);
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

		void drawImage(Texture *texture, float x, float y, float width, float height);

		void onFlush();

		// TODO: Function -> Draw rotated texture

		// TODO: Function -> draw texture with texture size
	};



	
}



