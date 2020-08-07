#pragma once

#include <string>

namespace River {
	class Texture {

	private:
	
		// OpenGL texture id
		unsigned int id;
		std::string filePath;

		int width;
		int height;
		int channels;
		
	public:
		Texture(std::string filePath);
		~Texture();

		unsigned int getId() { return id; }

	};
}


