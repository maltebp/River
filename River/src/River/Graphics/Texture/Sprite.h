#pragma once

#include <string>

#include "Texture.h"

namespace River {

	class Sprite {
	
	private:
		Texture *texture;
		bool dedicatedTexture = false;
		Texture::SampleCoordinates textureCoordinates;
		Texture::SampleCoordinates flippedCoordinates;

		unsigned int width, height;

		bool verticallyFlipped = false;
		bool horizontallyFlipped = false;

		
	private:
		Sprite(const Sprite &other);
		Sprite &operator=(const Sprite &);
		
		void updateAdjustedCoordinates();

	public:
		Sprite(Texture *texture, unsigned int textureOffsetX = 0, unsigned int textureOffsetY = 0, unsigned int textureWidth = 0, unsigned int textureHeight = 0, bool dedicatedTexture=false);
		
		/**
		 * @brief	Creates a new Sprite with a dedicated Texture (texture is owned, and only used by this sprite).
		*/
		Sprite(const std::string &texturePath, unsigned int textureOffsetX = 0, unsigned int textureOffsetY = 0, unsigned int textureWidth = 0, unsigned int textureHeight = 0);
		~Sprite();

		/**
		 * @brief The non-normalized Sprite width
		*/
		unsigned int getWidth();

		/**
		 * @brief The non-normalized Sprite width
		*/
		unsigned int getHeight();
		
		Texture* getTexture() const;


		void flipVertically();

		void flipHorizontally();

		void rotate(int times);

		const Texture::SampleCoordinates& getTextureCoordinates() const;

		/**
		 * @brief	Whether or not this Sprite's texture is only used by this Sprite. If so, the texture will be deleted
					with this Sprite
		*/
		bool hasDedicatedTexture();

		
	};

}


