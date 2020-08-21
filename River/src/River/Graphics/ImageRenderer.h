#pragma once

#include "Renderer.h"
#include "Texture/Sprite.h"
#include "Texture/Texture.h"
#include "Texture/TextureBinder.h"
#include "Window.h"
#include "Shader/ShaderProgram.h"
#include "VertexArray.h"


namespace River {

	// Vertex Array ------------------------------------------
	struct ImageVertex {
		GLfloat x, y, z;
		GLfloat textureSlot;
		GLfloat textureX, textureY;
		GLfloat numTextureChannels; // Consider to move this to a uniform
	};

	class ImageVertexArray : public VertexArray<ImageVertex> {
	protected:
		void setupAttributes() override {
			addAttributeFloat(3); // Position
			addAttributeFloat(1);  // Texture slot
			addAttributeFloat(2); // Tex position
			addAttributeFloat(1); // Num channels
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

		void drawSprite(const Sprite *sprite, float x, float y, float z, float width, float height, float rotation);

		void onFlush();

		// TODO: Function -> Draw rotated texture

		// TODO: Function -> draw texture with texture size
	};



	
}



