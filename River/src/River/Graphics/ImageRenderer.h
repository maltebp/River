#pragma once

#include "Renderer.h"
#include "Texture.h"
#include "Window.h"
#include "Shader/ShaderProgram.h"


namespace River {
	
	class ImageRenderer : public Renderer {

	private:
		unsigned int vertexArray;
		unsigned int vertexBuffer;
		unsigned int indexBuffer;

		ShaderProgram* shaderProgram;

	public:

		ImageRenderer(Window *window);

		void drawImage(Texture *texture, float x, float y, float width, float height);

		void flush();

		// TODO: Function -> Draw rotated texture

		// TODO: Function -> draw texture with texture size
	};

}



