#include "Texture.h"

namespace River {


	/*Texture::Texture(Image *texture, bool partiallyTransparent, unsigned int textureOffsetX, unsigned int textureOffsetY, unsigned int textureWidth, unsigned int textureHeight) {
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
	}*/


	/*Texture::Texture(const std::string& imagePath, bool partiallyTransparent, unsigned int textureOffsetX, unsigned int textureOffsetY, unsigned int textureWidth, unsigned int textureHeight) :
		Texture(Image::create(imagePath).setPartiallyTransparent(partiallyTransparent).finish(), textureOffsetX, textureOffsetY, textureWidth, textureHeight)
	{
		this->partiallyTransparent = partiallyTransparent;
		dedicatedImage = true;
	}*/

	Texture::Texture() {

	}



	Texture::~Texture() {
		/*if( dedicatedImage )
			delete image;*/
	}

	unsigned int Texture::getWidth() {
		if( width == 0 )
			// The texture should use full width of image, and it hasn't been
			// calculated yet, as it hasn't been loaded.
			throw AssetNotLoaded("The Texture asset must be loaded in order to retrieve its width");
		return width;
	}

	unsigned int Texture::getHeight() {
		if( height == 0 )
			// The texture should use full width of image, and it hasn't been
			// calculated yet, as it hasn't been loaded.
			throw AssetNotLoaded("The Texture asset must be loaded in order to retrieve its height");
		return height;
	}



	double Texture::getAspectRatio() {
		return width / (double) height;
	}

	Image *Texture::getImage() const {
		return image;
	}

	

	//void Texture::flipVertically() {
	//	verticallyFlipped = true;
	//	updateAdjustedCoordinates();
	//}

	//void Texture::flipHorizontally() {
	//	horizontallyFlipped = true;
	//	updateAdjustedCoordinates();
	//}

	//void Texture::updateAdjustedCoordinates() {

	//	if( horizontallyFlipped ) {
	//		flippedCoordinates.x1 = textureCoordinates.x2;
	//		flippedCoordinates.x2 = textureCoordinates.x1;
	//	} else {  
	//		flippedCoordinates.x1 = textureCoordinates.x1;
	//		flippedCoordinates.x2 = textureCoordinates.x2;
	//	}		   
	//	if( verticallyFlipped ) {
	//		flippedCoordinates.y1 = textureCoordinates.y2;
	//		flippedCoordinates.y2 = textureCoordinates.y1;
	//	}else {	 
	//		flippedCoordinates.y1 = textureCoordinates.y1;
	//		flippedCoordinates.y2 = textureCoordinates.y2;
	//	}
	//}

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
			throw new InvalidAssetStateException("Texture sampling coordinates (offsets, size) goes beyond image coordinates½");


		// Adjust sampling coordinates to actual image coordinates,
		// now that they're loaded. Only if the sampling size is
		// set to full image size, of course (indicated by second coordinate
		// being less than or equal to first coordinate)

		if( textureCoordinates.x2 <= textureCoordinates.x1 )
			textureCoordinates.x2 = image->getWidth();

		if( textureCoordinates.y2 <= textureCoordinates.y1 )
			textureCoordinates.y2 = image->getHeight();


		width = textureCoordinates.x2 - textureCoordinates.x1; 
		height = textureCoordinates.y2 - textureCoordinates.y1; 

		// Normalize the sampling coordinates
		textureCoordinates.x1 = image->normalizeX(textureCoordinates.x1);
		textureCoordinates.x2 = image->normalizeX(textureCoordinates.x2);
		textureCoordinates.y1 = image->normalizeY(textureCoordinates.y1);
		textureCoordinates.y2 = image->normalizeY(textureCoordinates.y2);
	}


	void Texture::onUnload() {
		image->unload();
		width = 0;
		height = 0;
	}


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Creator functions


	Texture::Creator::Creator(Image* source, bool dedicatedImage) {
		texture = new Texture();
		texture->image = source;
		texture->partiallyTransparent = source->isPartiallyTransparent();
		texture->dedicatedImage = dedicatedImage;

		// Sample x2/y2 of 0 means "from offset x/y to full width/height".
		// However these coordinates can only be set once the image has been loaded.
		texture->textureCoordinates.x1 = 0;
		texture->textureCoordinates.x2 = 0;
		texture->textureCoordinates.y1 = 0;
		texture->textureCoordinates.y2 = 0;
	}

	
	Texture::Creator& Texture::Creator::setSamplingOffset(unsigned int x, unsigned int y) {
		auto width = texture->textureCoordinates.x2 - texture->textureCoordinates.x1;
		texture->textureCoordinates.x1 = x;

		auto height = texture->textureCoordinates.y2 - texture->textureCoordinates.y1;
		texture->textureCoordinates.y1 = y;

		// Only if the width/height is larger than zero (means it shouldn't be full width)
		// we move the second coordinate, so the width/height remains the same
		if( width > 0 ) texture->textureCoordinates.x2 = x + width; 
		if( height > 0 ) texture->textureCoordinates.y2 = y + height; 

		return *this;
	}


	Texture::Creator& Texture::Creator::setSamplingSize(unsigned int width, unsigned height) {
		texture->textureCoordinates.x2 = texture->textureCoordinates.x1 + width;
		texture->textureCoordinates.y2 = texture->textureCoordinates.y1 + height;

		return *this;

	}


	Texture::Creator& Texture::Creator::setPartiallyTransparent() {
		texture->partiallyTransparent = true;

		return *this;
	}

	Texture::Creator& Texture::Creator::setAssetCollection(AssetCollection* collection) {
		this->collection = collection;
		return *this;
	}


	Texture* Texture::Creator::finish() {
		if( texture == nullptr )
			throw new InvalidStateException("Texture Creator has already returned its Texture");

		if( collection != nullptr ) collection->add(texture);
		
		auto finishedTexture = texture;
		texture = nullptr;
		return finishedTexture;
	}

}