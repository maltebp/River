#pragma once

#include <string>

#include "Image.h"

namespace River {


	/**
	 * @brief	A Texture defines a drawable part of some Image. This can be the enitre Image or just a specified region of it.
	*/
	class Texture {
	public:

		/**
		 * @brief 
		 * @param partiallyTransparent	Whether or not the Texture contains partially transparent pixels. Even though the Image may, it doens't mean that the Texture does
		 * @return 
		*/
		Texture(Image *image, bool partiallyTransparent, unsigned int textureOffsetX = 0, unsigned int textureOffsetY = 0, unsigned int textureWidth = 0, unsigned int textureHeight = 0);
		
		/**
		 * @brief	Creates a new Texture with a dedicated Image (texture is owned, and only used by this sprite).
		 * @param partiallyTransparent    Whether or not the dedicated Image contains partially transparent pixels
		*/
		Texture(const std::string &imagePath, bool partiallyTransparent, unsigned int textureOffsetX = 0, unsigned int textureOffsetY = 0, unsigned int textureWidth = 0, unsigned int textureHeight = 0);
		
		~Texture();

		/**
		 * @brief The non-normalized Texture width
		*/
		unsigned int getWidth();

		/**
		 * @brief The non-normalized Texture width
		*/
		unsigned int getHeight();
		
		Image* getImage() const;


		void flipVertically();

		void flipHorizontally();

		/**
		 * @brief 
		 
		*/
		void rotate(int times);

		const Image::SampleCoordinates& getTextureCoordinates() const;

		/**
		 * @brief	Whether or not this Texture's texture is only used by this Texture. If so, the texture will be deleted
					with this Texture
		*/
		bool hasDedicatedImage() const;


		/**
		 * @brief	Checks whether or not this texture contains partially transparent pixels
		*/
		bool isPartiallyTransparent() const;




	private:
		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture&) = delete;

		void updateAdjustedCoordinates();


	private:
		Image* image;
		bool dedicatedImage = false;
		Image::SampleCoordinates textureCoordinates;
		Image::SampleCoordinates flippedCoordinates;

		// Whether or not this texture contains partially transparent areas
		bool partiallyTransparent;

		unsigned int width, height;

		bool verticallyFlipped = false;
		bool horizontallyFlipped = false;
		
	};

}


