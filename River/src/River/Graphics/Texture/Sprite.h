#pragma once

#include <string>

#include "Texture.h"

namespace River {

	class Sprite {
	public:
		// Holds data about which part of the texture is to be sampled
		struct TextureCoordinates {
			float x1; // Left
			float y1; // Top
			float x2; // Right
			float y2; // Bottom
		};
	
	private:
		Texture *texture;
		bool dedicatedTexture = false;
		TextureCoordinates textureCoordinates;

		unsigned int width, height;

		
	private:
		Sprite(const Sprite &other);
		Sprite &operator=(const Sprite &);
	public:
		Sprite(Texture *texture, unsigned int textureOffsetX = 0, unsigned int textureOffsetY = 0, unsigned int textureWidth = 0, unsigned int textureHeight = 0);
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
		
		Texture *getTexture();

		bool hasDedicatedTexture();

		const TextureCoordinates& getTextureCoordinates() const;
	};

}


