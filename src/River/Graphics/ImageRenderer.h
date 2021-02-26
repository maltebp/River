#pragma once

#include "River/Graphics/Camera/Camera.h"
#include "Texture/Sprite.h"
#include "Texture/Texture.h"
#include "Texture/ImageBinder.h"
#include "Window.h"
#include "Shader/ShaderProgram.h"
#include "VertexArray.h"
#include "River/Primitives/Color.h"


namespace River {


	// ImageRenderer -----------------------------------------
	class ImageRenderer {
	public:
		struct TextureData {
			Image* texture = nullptr;
			Image::SampleCoordinates textureCoordinates = { 0, 0, 1, 1 };
		};

		struct TransformData {
			float x=0, y=0, z=0;
			float width=0, height=0;
			float rotation=0;
			float opacity = 1.0;
			Color color = Colors::WHITE;
		};


	public:
		ImageRenderer();

		void drawRectangle(const TransformData& transformData);
		void drawSprite(const Texture *sprite, const TransformData& transformData);
		void drawTexture(const TextureData& textureData, const TransformData& transformData);

		void enableBlending();
		void disableBlending();

		void setCamera(Camera* camera);

		void flush();

		// TODO: Function -> Draw rotated texture

		// TODO: Function -> draw texture with texture size

	private:
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
				addAttributeFloat(1);  // Image slot
				addAttributeFloat(2); // Tex position
				addAttributeFloat(1); // Num channels
			}
		};


	private:
		ImageVertexArray vertexArray;

		ShaderProgram* shaderProgram;
		ImageBinder textureBinder;

		bool blending = false;

		Camera* camera = nullptr;
	};



	
}



