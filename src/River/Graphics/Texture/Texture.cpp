#include "Texture.h"

namespace River {


	Texture::Texture(Image *texture, bool partiallyTransparent, unsigned int textureOffsetX, unsigned int textureOffsetY, unsigned int textureWidth, unsigned int textureHeight) {
		this->image = texture;
		this->dedicatedImage = false;
		
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
	
		this->partiallyTransparent = partiallyTransparent;
	}


	Texture::Texture(const std::string& imagePath, bool partiallyTransparent, unsigned int textureOffsetX, unsigned int textureOffsetY, unsigned int textureWidth, unsigned int textureHeight) :
		Texture(new Image(imagePath, partiallyTransparent), textureOffsetX, textureOffsetY, textureWidth, textureHeight)
	{
		this->partiallyTransparent = partiallyTransparent;
		dedicatedImage = true;
	}


	Texture::~Texture() {
		if( dedicatedImage )
			delete image;
	}

	unsigned int Texture::getWidth() {
		return width;
	}

	unsigned int Texture::getHeight() {
		return height;
	}

	Image *Texture::getImage() const {
		return image;
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

	bool Texture::hasDedicatedImage() const {
		return dedicatedImage;
	}

	const Image::SampleCoordinates& Texture::getTextureCoordinates() const {
		return flippedCoordinates;
	}


	bool Texture::isPartiallyTransparent() const {
		return partiallyTransparent;
	}

}