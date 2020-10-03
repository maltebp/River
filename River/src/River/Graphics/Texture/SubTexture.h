#pragma once

#include <string>

#include "Texture.h"

namespace River {

	class SubTexture {
	
	private:
		Texture *texture;
		bool dedicatedTexture = false;
		Texture::SampleCoordinates textureCoordinates;
		Texture::SampleCoordinates flippedCoordinates;

		unsigned int width, height;

		bool verticallyFlipped = false;
		bool horizontallyFlipped = false;

		
	private:
		SubTexture(const SubTexture &other);
		SubTexture &operator=(const SubTexture &);
		
		void updateAdjustedCoordinates();

	public:
		SubTexture(Texture *texture, unsigned int textureOffsetX = 0, unsigned int textureOffsetY = 0, unsigned int textureWidth = 0, unsigned int textureHeight = 0, bool dedicatedTexture=false);
		
		/**
		 * @brief	Creates a new SubTexture with a dedicated Texture (texture is owned, and only used by this sprite).
		*/
		SubTexture(const std::string &texturePath, unsigned int textureOffsetX = 0, unsigned int textureOffsetY = 0, unsigned int textureWidth = 0, unsigned int textureHeight = 0);
		~SubTexture();

		/**
		 * @brief The non-normalized SubTexture width
		*/
		unsigned int getWidth();

		/**
		 * @brief The non-normalized SubTexture width
		*/
		unsigned int getHeight();
		
		Texture* getTexture() const;


		void flipVertically();

		void flipHorizontally();

		void rotate(int times);

		const Texture::SampleCoordinates& getTextureCoordinates() const;

		/**
		 * @brief	Whether or not this SubTexture's texture is only used by this SubTexture. If so, the texture will be deleted
					with this SubTexture
		*/
		bool hasDedicatedTexture();

		
	};

}


