#include "SubTexture.h"

namespace River {

	SubTexture::SubTexture(Texture *texture, unsigned int textureOffsetX, unsigned int textureOffsetY, unsigned int textureWidth, unsigned int textureHeight, bool dedicatedTexture) {
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

	SubTexture::SubTexture(const std::string &texturePath, unsigned int textureOffsetX, unsigned int textureOffsetY, unsigned int textureWidth, unsigned int textureHeight) :
		SubTexture(new Texture(texturePath), textureOffsetX, textureOffsetY, textureWidth, textureHeight)
	{}

	SubTexture::~SubTexture() {
		if( dedicatedTexture )
			delete texture;
	}

	unsigned int SubTexture::getWidth() {
		return width;
	}

	unsigned int SubTexture::getHeight() {
		return height;
	}

	Texture *SubTexture::getTexture() const {
		return texture;
	}


	void SubTexture::flipVertically() {
		verticallyFlipped = true;
		updateAdjustedCoordinates();
	}

	void SubTexture::flipHorizontally() {
		horizontallyFlipped = true;
		updateAdjustedCoordinates();
	}

	void SubTexture::updateAdjustedCoordinates() {

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

	bool SubTexture::hasDedicatedTexture() {
		return dedicatedTexture;
	}

	const Texture::SampleCoordinates& SubTexture::getTextureCoordinates() const {
		return flippedCoordinates;
	}


}