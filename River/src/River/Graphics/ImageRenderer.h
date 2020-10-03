#pragma once

#include "Renderer.h"
#include "Texture/Sprite.h"
#include "Texture/Texture.h"
#include "Texture/TextureBinder.h"
#include "Window.h"
#include "Shader/ShaderProgram.h"
#include "VertexArray.h"
#include "Color.h"


namespace River {

	// Vertex Array ------------------------------------------
	struct ImageVertex {
		GLfloat x, y, z;
		GLfloat r, g, b, a;
		GLfloat textureSlot;
		GLfloat textureX, textureY;
		GLfloat numTextureChannels; // Consider to move this to a uniform
	};

	class ImageVertexArray : public VertexArray<ImageVertex> {
	protected:
		void setupAttributes() override {
			addAttributeFloat(3); // Position
			addAttributeFloat(4); // Color
			addAttributeFloat(1);  // Texture slot
			addAttributeFloat(2); // Tex position
			addAttributeFloat(1); // Num channels
		}
	};


	// ImageRenderer -----------------------------------------
	class ImageRenderer : public Renderer {
	public:

		struct TextureData {
			Texture* texture = nullptr;
			Texture::SampleCoordinates textureCoordinates = { 0, 0, 1, 1 };
		};

		struct TransformData {
			float x=0, y=0, z=0;
			float width=0, height=0;
			float rotation=0;
			Color color = Colors::WHITE;
		};

	private:
		ImageVertexArray vertexArray;

		ShaderProgram* shaderProgram;
		TextureBinder textureBinder;

	public:

		ImageRenderer(Window *window);

		void drawRectangle(const TransformData& transformData);
		void drawSprite(const SubTexture *sprite, const TransformData& transformData);
		void drawTexture(const TextureData& textureData, const TransformData& transformData);

		void onFlush();

		// TODO: Function -> Draw rotated texture

		// TODO: Function -> draw texture with texture size


	private:

		
	};



	
}



