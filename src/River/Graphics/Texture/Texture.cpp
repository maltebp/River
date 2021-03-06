#include "River/pch.h"

#include "Texture.h"

namespace River {



	Texture::Texture() {}


	void Texture::destroy() {
		if( isLoaded() )
			throw new AssetDeletionFailedException("Texture asset is still loaded, and cannot be destroyed");

		if( dedicatedImage )
			image->destroy();

		delete this;
	}


	Texture::~Texture() {}



	unsigned int Texture::getWidth() const {
		if( width == 0 )
			// The texture should use full width of image, and it hasn't been
			// calculated yet, as it hasn't been loaded.
			throw AssetNotLoaded("The Texture asset must be loaded in order to retrieve its width");
		return width;
	}

	unsigned int Texture::getHeight() const {
		if( height == 0 )
			// The texture should use full width of image, and it hasn't been
			// calculated yet, as it hasn't been loaded.
			throw AssetNotLoaded("The Texture asset must be loaded in order to retrieve its height");
		return height;
	}



	double Texture::getAspectRatio() const {
		return width / (double) height;
	}

	Image *Texture::getImage() const {
		return image;
	}

	bool Texture::hasDedicatedImage() const {
		return dedicatedImage;
	}

	const Image::SampleCoordinates& Texture::getTextureCoordinates() const {
		return textureCoordinates;
		//return flippedCoordinates;
	}


	bool Texture::isPartiallyTransparent() const {
		return partiallyTransparent;
	}


	void Texture::onLoad() {
		image->load();
	
		// Adjust texture coordinates to correct coordinates
		
		if( textureCoordinates.x1 >= image->getWidth()	||
			textureCoordinates.y1 >= image->getHeight()	||
			textureCoordinates.x2 > image->getWidth()	||
			textureCoordinates.y2 > image->getHeight()
		)
			throw new InvalidAssetStateException("Texture sampling coordinates (offsets, size) goes beyond image coordinates");


		// Adjust sampling coordinates to actual image coordinates,
		// now that they're loaded. Only if the sampling size is
		// set to full image size, of course (indicated by second coordinate
		// being less than or equal to first coordinate)

		if( textureCoordinates.x2 <= textureCoordinates.x1 )
			textureCoordinates.x2 = (float)image->getWidth();

		if( textureCoordinates.y2 <= textureCoordinates.y1 )
			textureCoordinates.y2 = (float)image->getHeight();


		width = (unsigned int)(textureCoordinates.x2 - textureCoordinates.x1); 
		height = (unsigned int)(textureCoordinates.y2 - textureCoordinates.y1); 

		// Normalize the sampling coordinates
		textureCoordinates.x1 = image->normalizeX((unsigned int)textureCoordinates.x1);
		textureCoordinates.x2 = image->normalizeX((unsigned int)textureCoordinates.x2);
		textureCoordinates.y1 = image->normalizeY((unsigned int)textureCoordinates.y1);
		textureCoordinates.y2 = image->normalizeY((unsigned int)textureCoordinates.y2);
	}


	void Texture::onUnload() {
		image->unload();
		width = 0;
		height = 0;
	}


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Creator functions


	Texture::Creator::Creator(Image* source, bool dedicatedImage) {
		asset = new Texture();
		asset->image = source;
		asset->partiallyTransparent = source->isPartiallyTransparent();
		asset->dedicatedImage = dedicatedImage;

		// Sample x2/y2 of 0 means "from offset x/y to full width/height".
		// However these coordinates can only be set once the image has been loaded.
		asset->textureCoordinates.x1 = 0;
		asset->textureCoordinates.x2 = 0;
		asset->textureCoordinates.y1 = 0;
		asset->textureCoordinates.y2 = 0;
	}

	
	Texture::Creator& Texture::Creator::setSamplingOffset(unsigned int x, unsigned int y) {
		auto width = asset->textureCoordinates.x2 - asset->textureCoordinates.x1;
		asset->textureCoordinates.x1 = (float)x;

		auto height = asset->textureCoordinates.y2 - asset->textureCoordinates.y1;
		asset->textureCoordinates.y1 = (float)y;

		// Only if the width/height is larger than zero (means it shouldn't be full width)
		// we move the second coordinate, so the width/height remains the same
		if( width > 0 ) asset->textureCoordinates.x2 = x + width; 
		if( height > 0 ) asset->textureCoordinates.y2 = y + height; 

		return *this;
	}


	Texture::Creator& Texture::Creator::setSamplingSize(unsigned int width, unsigned height) {
		asset->textureCoordinates.x2 = asset->textureCoordinates.x1 + width;
		asset->textureCoordinates.y2 = asset->textureCoordinates.y1 + height;

		return *this;

	}


	Texture::Creator& Texture::Creator::setPartiallyTransparent() {
		asset->partiallyTransparent = true;

		return *this;
	}

}