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
		updateAdjustedCoordinates(); 

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


	void Sprite::flipVertically() {
		verticallyFlipped = true;
		updateAdjustedCoordinates();
	}

	void Sprite::flipHorizontally() {
		horizontallyFlipped = true;
		updateAdjustedCoordinates();
	}

	void Sprite::updateAdjustedCoordinates() {

		if( horizontallyFlipped ) {
			flippedCoordinates.x1 = textureCoordinates.x2;
			flippedCoordinates.x2 = textureCoordinates.x1;
		} else {  
			flippedCoordinates.x1 = textureCoordinates.x1;
			flippedCoordinates.x2 = textureCoordinates.x2;
		}		   
		if( verticallyFlipped ) {
			flippedCoordinates.y1 = textureCoordinates.y2;
			flippedCoordinates.y2 = textureCoordinates.y1;
		}else {	 
			flippedCoordinates.y1 = textureCoordinates.y1;
			flippedCoordinates.y2 = textureCoordinates.y2;
		}
	}

	bool Sprite::hasDedicatedTexture() {
		return dedicatedTexture;
	}

	const Texture::SampleCoordinates& Sprite::getTextureCoordinates() const {
		return flippedCoordinates;
	}


}