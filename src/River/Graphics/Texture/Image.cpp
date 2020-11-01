#include "Image.h"

#include <River/Vendor/stb_image/stb_image.h>
#include "../GL.h"
#include "River/Error.h"

namespace River {


	void Image::onLoad() {

		// If not from file, the image data will always be loaded, so we do nothing here
		if( !fromFile ) return;

		// TODO: Determine the correct alignment from the image file
		rowAlignment = 4;

		// Load texture file
		//stbi_set_flip_vertically_on_load(true);
		unsigned char* pixels = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		if( !pixels ) {
			throw new River::AssetNotFoundException(filePath);
		}

		createGLTexture(pixels);
		stbi_image_free(pixels);

		// Optimization (no reason to make it partially transparent if there is no alpha channel)
		this->partiallyTransparent = this->channels == 4 && partiallyTransparent;
	}


	void Image::createGLTexture(void *data) {

		GL(glPixelStorei(GL_UNPACK_ALIGNMENT, rowAlignment));

		GL(glGenTextures(1, &id));
		GL(glBindTexture(GL_TEXTURE_2D, id));

		// Image filtering/wrap options
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));

		// I assume this setting is bound to the texture and not the slot (texture unit)
		GLenum glFilter;
		if( scaleMode == ScaleMode::LINEAR )
			glFilter = GL_LINEAR;
		if( scaleMode == ScaleMode::NEAREST )
			glFilter = GL_NEAREST;

		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter));

		GLenum format;
		GLint internalFormat;

		if( channels != 1 && channels != 3 && channels != 4 )
			throw new River::AssetException("Texture does not have 3 or 4 channels, but " + channels);
		if( channels == 1 ) {
			format = GL_RED;
			internalFormat = GL_RED;
		}
		if( channels == 3 ) {
			format = GL_RGB;
			internalFormat = GL_RGB8;
		}
		if( channels == 4 ) {
			format = GL_RGBA;
			internalFormat = GL_RGBA8;
		}

		// Set data
		GL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data));

		//GL(glGenerateMipmap(GL_TEXTURE_2D));

		GL(glBindTexture(GL_TEXTURE_2D, 0));
	}



	void Image::onUnload() {

		// If not from file, the image cannot be unloaded without permantly destroying its
		// data, as it has been created in the code somewhere. So we do nothing.
		if( fromFile ) return;

		if( glIsTexture(id) == GL_TRUE )
			GL(glDeleteTextures(1, &id));
		// Automatically unbinds the texture as well
		// and data is associated with the gl texture, so this should free all
		// memory associated with the image
	}


	void Image::bind(unsigned int textureSlot) {
		if( !isLoaded() )
			throw new AssetNotLoaded("Image must be loaded in order to be bound");

		// Note: Not using glBindTextureUnit as this is only available from 4.5 and onwards
		GL(
			glActiveTexture(GL_TEXTURE0 + textureSlot);
			glBindTexture(GL_TEXTURE_2D, id);
		);
	}	

	unsigned int Image::getNumChannels() {
		if( !isLoaded() )
			throw new AssetNotLoaded("Image must be loaded to check the number of channels of it");
		return channels;
	}

	unsigned int Image::getWidth() {
		if( !isLoaded() )
			throw new AssetNotLoaded("Image must be loaded to check the width of it");
		return width;
	}

	unsigned int Image::getHeight() {
		if( !isLoaded() )
			throw new AssetNotLoaded("Image must be loaded to check the height of it");
		return height;
	}

	// Comment on getHeight and getWidth:
	// The fact that the Image must be loaded is in fact only a problem when the image is
	// from a file. As such we can't know the width/height of the image before it has been
	// loaded.
	// However, for simplicity sake, I decided to make it so that any Image must be loaded
	// in order to check the width / height (to prevent more confusion)


	Image *Image::getWhiteTexture() {
		if( whiteTexture == nullptr ) {
			unsigned char data = 0xFF;
			whiteTexture = Image::create(&data, 1, 1, 1, 1).finish();
			whiteTexture->load(); // The white texture should always be loaded
		}
		return whiteTexture;
	}

	float Image::normalizeX(unsigned int coordinate) {
		return (float) coordinate / (float) width;
	}

	float Image::normalizeY(unsigned int coordinate) {
		return (float) coordinate / (float) height;
	}


	bool Image::isPartiallyTransparent() const {
		return partiallyTransparent;
	}


	Image::Creator::Creator(const std::string& filePath) {
		image = new Image();
		image->filePath = filePath;
		image->fromFile = true;
	}


	/**
	 * @brief	Constructs an Image from a data source. The image will copy the data on creation, and unloading this image will have no effect on memory.
	*/
	Image::Creator::Creator(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels, unsigned int rowAlignment) {
		image = new Image();
		image->fromFile = false;
		image->width = width;
		image->height = height;
		image->channels = channels;
		image->rowAlignment = rowAlignment;
		this->imageData = data;
	}


	Image* Image::Creator::finish() {
		if( assetCollection != nullptr ) assetCollection->add(image);

		if( !image->fromFile ) {
			// The image should always be "loaded" when not from file
			// so we construct it here
			image->createGLTexture(imageData);
		}

		return image;
	}

	Image::Creator& Image::Creator::setPartiallyTransparent(bool toggle) {
		image->partiallyTransparent = toggle;
		return *this;
	}

	Image::Creator& Image::Creator::setScaleMode(ScaleMode mode) {
		image->scaleMode = mode;
		return *this;
	}

	Image::Creator& Image::Creator::setAssetCollection(AssetCollection* collection) {
		assetCollection = collection;
		return *this;
	}


}