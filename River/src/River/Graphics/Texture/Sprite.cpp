#include "Sprite.h"

namespace River {

	Sprite::Sprite(Texture *texture, unsigned int textureOffsetX, unsigned int textureOffsetY, unsigned int textureWidth, unsigned int textureHeight, bool dedicatedTexture) {
		this->texture = texture;
		this->dedicatedTexture = dedicatedTexture;
		
		if( textureWidth == 0 ) textureWidth = texture->getWidth();
		if( textureHeight == 0 ) textureHeight = texture->getHeight();

		textureCoordinates = {
			texture->normalizeX(textureOffsetX),
			texture->normalizeY(textureOffsetY),
			texture->normalizeX(textureOffsetX+textureWidth),
			texture->normalizeY(textureOffsetY+textureHeight)
		};

		this->width = textureWidth;
		this->height = textureHeight;
	}

	Sprite::Sprite(const std::string &texturePath, unsigned int textureOffsetX, unsigned int textureOffsetY, unsigned int textureWidth, unsigned int textureHeight) :
		Sprite(new Texture(texturePath), textureOffsetX, textureOffsetY, textureWidth, textureHeight)
	{}

	Sprite::~Sprite() {
		if( dedicatedTexture )
			delete texture;
	}

	unsigned int Sprite::getWidth() {
		return width;
	}

	unsigned int Sprite::getHeight() {
		return height;
	}

	Texture *Sprite::getTexture() const {
		return texture;
	}

	bool Sprite::hasDedicatedTexture() {
		return dedicatedTexture;
	}

	const Sprite::TextureCoordinates& Sprite::getTextureCoordinates() const {
		return textureCoordinates;
	}


}