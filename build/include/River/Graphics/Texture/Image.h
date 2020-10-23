#pragma once

#include <string>

#include "../GL.h"

namespace River {

	class Image {
	public:
		struct SampleCoordinates {
			float x1; // Left
			float y1; // Top
			float x2; // Right
			float y2; // Bottom
		};


		enum class FilterMode {
			LINEAR, NEAREST
		};


	

	private:
		void createGLTexture(void *data);

		// Prevent copying and assignemnt 
		Image(const Image &temp_obj) = delete;
		Image &operator=(const Image &temp_obj) = delete;


	public:
		Image(std::string filePath, bool partiallyTransparent);
		Image(unsigned int width, unsigned int height, unsigned int channels, bool partiallyTransparent, unsigned int rowAlignment, void *data);
		~Image();


		void bind(unsigned int textureSlot);
		unsigned int getId() { return id; }

		unsigned int getNumChannels();

		static Image *getWhiteTexture();

		unsigned int getWidth();
		unsigned int getHeight();

		void setFilterMode(FilterMode filter);
		

		/**
		 * @brief Normalizes the x-coordinate into the OpenGL coordinates (ranging from 0 to 1);
		*/
		float normalizeX(unsigned int coordinate);


		/**
		 * @brief Normalizes the x-coordinate into the OpenGL coordinates (ranging from 0 to 1);
		*/
		float normalizeY(unsigned int coordinate);
	

	private:
		static Image* whiteTexture;

		// OpenGL texture id
		unsigned int id;
		std::string filePath = "Unknown path";

		int width;
		int height;
		int channels;
		int rowAlignment;

		/**
		 * @brief	Whether or not this texture contains any partially transparent pixels. Curently, it's up to the user to
		 *			decide when constructing an texture
		*/
		bool partiallyTransparent;
	
	};
}


