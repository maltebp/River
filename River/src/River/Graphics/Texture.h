#pragma once

#include <iostream> //TODO: Delete this
#include <string>

#include "GL.h"

namespace River {

	class Texture {

	private:

		static Texture* whiteTexture;
	
		// OpenGL texture id
		unsigned int id;
		std::string filePath = "Unknown path";

		int width;
		int height;
		int channels;

	private:
		void createGLTexture(void* data);

		// Prevent copying and assignemnt 
		Texture(const Texture& temp_obj){}
		Texture& operator=(const Texture& temp_obj){}


	public:
		Texture(std::string filePath);
		Texture(unsigned int width, unsigned int height, unsigned int channels, void* data);
		~Texture();


		void bind(unsigned int textureSlot);
		unsigned int getId() { return id; }

		static Texture* getWhiteTexture();
	};
}


