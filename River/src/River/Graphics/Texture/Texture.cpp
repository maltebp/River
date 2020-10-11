#include "Texture.h"

namespace River {

	Texture::Texture(Image *texture, unsigned int textureOffsetX, unsigned int textureOffsetY, unsigned int textureWidth, unsigned int textureHeight, bool dedicatedTexture) {
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

	Texture::Texture(const std::string &texturePath, unsigned int textureOffsetX, unsigned int textureOffsetY, unsigned int textureWidth, unsigned int textureHeight) :
		Texture(new Image(texturePath), textureOffsetX, textureOffsetY, textureWidth, textureHeight)
	{}

	Texture::~Texture() {
		if( dedicatedTexture )
			delete texture;
	}

	unsigned int Texture::getWidth() {
		return width;
	}

	unsigned int Texture::getHeight() {
		return height;
	}

	Image *Texture::getTexture() const {
		return texture;
	}


	void Texture::flipVertically() {
		verticallyFlipped = true;
		updateAdjustedCoordinates();
	}

	void Texture::flipHorizontally() {
		horizontallyFlipped = true;
		updateAdjustedCoordinates();
	}

	void Texture::updateAdjustedCoordinates() {

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

	bool Texture::hasDedicatedTexture() {
		return dedicatedTexture;
	}

	const Image::SampleCoordinates& Texture::getTextureCoordinates() const {
		return flippedCoordinates;
	}


}