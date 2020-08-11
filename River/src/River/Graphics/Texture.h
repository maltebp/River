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
		std::string filePath;

		int width;
		int height;
		int channels;

	public:
		Texture(std::string filePath);
		Texture(unsigned int width, unsigned int height, void* data);
		~Texture();

		Texture(const Texture& temp_obj){ std::cout << "ASsignment" << std::endl; }
		Texture& operator=(const Texture& temp_obj){ std::cout << "ASsignment" << std::endl; }


		void bind(unsigned int textureSlot);
		unsigned int getId() { return id; }

		static Texture* getWhiteTexture();
	};
}


