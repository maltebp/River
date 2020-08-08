#pragma once

#include "Renderer.h"
#include "Texture.h"
#include "Window.h"
#include "Shader/ShaderProgram.h"
#include "VertexArray.h"


namespace River {

	struct ImageVertex {
		GLfloat x, y, texX, texY;
	};

	class ImageVertexArray : public VertexArray<ImageVertex> {

	protected:
		void setupAttributes() override {
			addAttributeFloat(2); // Position
			addAttributeFloat(2); // Tex position
		}
	};


	
	class ImageRenderer : public Renderer {

	private:
		ImageVertexArray vertexArray;

		ShaderProgram* shaderProgram;

	public:

		ImageRenderer(Window *window);

		void drawImage(Texture *texture, float x, float y, float width, float height);

		void flush();

		// TODO: Function -> Draw rotated texture

		// TODO: Function -> draw texture with texture size
	};



	
}



