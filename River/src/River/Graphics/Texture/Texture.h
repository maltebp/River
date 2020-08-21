#pragma once

#include <string>

#include "../GL.h"

namespace River {

	class Texture {
	public:
		struct SampleCoordinates {
			float x1; // Left
			float y1; // Top
			float x2; // Right
			float y2; // Bottom
		};


	private:

		static Texture *whiteTexture;

		// OpenGL texture id
		unsigned int id;
		std::string filePath = "Unknown path";

		int width;
		int height;
		int channels;
		int rowAlignment;

	private:
		void createGLTexture(void *data);

		// Prevent copying and assignemnt 
		Texture(const Texture &temp_obj);
		Texture &operator=(const Texture &temp_obj);


	public:
		Texture(std::string filePath);
		Texture(unsigned int width, unsigned int height, unsigned int channels, unsigned int rowAlignment, void *data);
		~Texture();


		void bind(unsigned int textureSlot);
		unsigned int getId() { return id; }

		unsigned int getNumChannels();

		static Texture *getWhiteTexture();

		unsigned int getWidth();
		unsigned int getHeight();

		/**
		 * @brief Normalizes the x-coordinate into the OpenGL coordinates (ranging from 0 to 1);
		*/
		float normalizeX(unsigned int coordinate);

		/**
	 * @brief Normalizes the x-coordinate into the OpenGL coordinates (ranging from 0 to 1);
	*/
		float normalizeY(unsigned int coordinate);
	};
}


