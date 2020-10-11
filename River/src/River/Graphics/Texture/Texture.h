#pragma once

#include <string>

#include "Image.h"

namespace River {


	/**
	 * @brief	A Texture defines a drawable part of some image. This can be the enitre image or just a specified region of it.
	*/
	class Texture {
	
	private:
		Image *texture;
		bool dedicatedTexture = false;
		Image::SampleCoordinates textureCoordinates;
		Image::SampleCoordinates flippedCoordinates;

		unsigned int width, height;

		bool verticallyFlipped = false;
		bool horizontallyFlipped = false;

		
	private:
		Texture(const Texture &other);
		Texture &operator=(const Texture &);
		
		void updateAdjustedCoordinates();

	public:
		Texture(Image *texture, unsigned int textureOffsetX = 0, unsigned int textureOffsetY = 0, unsigned int textureWidth = 0, unsigned int textureHeight = 0, bool dedicatedTexture=false);
		
		/**
		 * @brief	Creates a new Texture with a dedicated Image (texture is owned, and only used by this sprite).
		*/
		Texture(const std::string &texturePath, unsigned int textureOffsetX = 0, unsigned int textureOffsetY = 0, unsigned int textureWidth = 0, unsigned int textureHeight = 0);
		~Texture();

		/**
		 * @brief The non-normalized Texture width
		*/
		unsigned int getWidth();

		/**
		 * @brief The non-normalized Texture width
		*/
		unsigned int getHeight();
		
		Image* getTexture() const;


		void flipVertically();

		void flipHorizontally();

		void rotate(int times);

		const Image::SampleCoordinates& getTextureCoordinates() const;

		/**
		 * @brief	Whether or not this Texture's texture is only used by this Texture. If so, the texture will be deleted
					with this Texture
		*/
		bool hasDedicatedTexture();

		
	};

}


