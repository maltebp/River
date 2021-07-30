#include "River/pch.h"

#include "Image.h"

#include "River/External/stb_image/stb_image.h"
#include "River/Error.h"


namespace River {


	using ScaleMode = Image::ScaleMode;
	using MipmapMode = Image::MipmapMode;
	using WrapMode = Image::WrapMode;


	Image::Image() { }


	Image::~Image() { }


	void Image::destroy() {
		if( isLoaded() )
			throw new AssetDeletionFailedException("Image asset is still loaded, and cannot be deleted");

		if( !fromFile ) {
			// Only the OpenGL textures of Images that are not loaded from files
			// persists beyond the onUnload function.
			auto result = GL(glIsTexture(id));
			if( result != GL_TRUE )
				// TODO: Convert this to warning
				throw new AssetDeletionFailedException("Image was not a Texture but it should be (this will be a warning in the future)");
			GL(glDeleteTextures(1, &id));

		}
		
		delete this;
	}


	void Image::onLoad() {

		// If not from file, the image data will always be loaded, so we do nothing here
		if( !fromFile ) return;

		// TODO: Determine the correct alignment from the image file
		rowAlignment = 4;

		// TODO: Check if colors are 1 byte large (and throw error if they are not)

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

		GLint currentTexture;
		GL(glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture));

		GL(glGenTextures(1, &id));
		GL(glBindTexture(GL_TEXTURE_2D, id));

		// Image filtering/wrap options
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toGLWrapMode(wrapModeHorizontal)));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toGLWrapMode(wrapModeVertical)));

		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLScaleMode(scaleDownMode, mipmapMode)));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLScaleMode(scaleUpMode)));

		// Note on format: We only allow for 1 byte colors, thus simplifying the options a little bit.
		// This is both the case for the loaded data, and the intenral texture format.

		GLenum format;
		GLint internalFormat;
        switch(channels) {
            // Note: We only allow 1 byte colors
            case 1:
				format = GL_RED;
				internalFormat = GL_R8;
				break;
			case 2:
				format = GL_RG;
				internalFormat = GL_RG8;
				break;
			case 3:
				format = GL_RGB;
				internalFormat = GL_RGB8;
				break;
			case 4:
				format = GL_RGBA;
				internalFormat = GL_RGBA8;
				break;
            default:
                throw new River::InvalidArgumentException("Image does not support more than 4 channels");
        }

		GL(glPixelStorei(GL_UNPACK_ALIGNMENT, rowAlignment));

		// Set data
		GL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data));

		if( mipmapMode != MipmapMode::NONE ) {
			GL(glGenerateMipmap(GL_TEXTURE_2D));
		}

		GL(glBindTexture(GL_TEXTURE_2D, currentTexture));
	}



	void Image::onUnload() {

		// If not from file, the image cannot be unloaded without permantly destroying its
		// data, as it has been created in the code somewhere. So we do nothing.
		if( !fromFile ) return;

		if( glIsTexture(id) != GL_TRUE ) return;

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


	GLenum Image::toGLScaleMode(Image::ScaleMode scaleMode, MipmapMode mipmapMode) {
		
		switch(mipmapMode) {
			case MipmapMode::NONE:
				switch(scaleMode) {
					case ScaleMode::LINEAR: return GL_LINEAR;
					case ScaleMode::NEAREST: return GL_NEAREST;
				}
				break;

			case MipmapMode::LINEAR:
				switch(scaleMode) {
					case ScaleMode::LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
					case ScaleMode::NEAREST: return GL_NEAREST_MIPMAP_LINEAR;
				}
				break;

			case MipmapMode::NEAREST:
				switch(scaleMode) {
					case ScaleMode::LINEAR: return GL_LINEAR_MIPMAP_NEAREST;
					case ScaleMode::NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
				}
				break;
		}

		throw NotImplementedException("Combination of ScaleMode and MipmapMode is not supported");
	}


	GLenum Image::toGLWrapMode(Image::WrapMode mode) {
		switch(mode) {
			case Image::WrapMode::NONE:
				return GL_CLAMP_TO_BORDER;
			case Image::WrapMode::REPEAT:
				return GL_REPEAT;
			case Image::WrapMode::CLAMP:
				return GL_CLAMP_TO_EDGE;
		}
		throw NotImplementedException("Wrap mode has not been implemented yet");
	}


	Image::Creator::Creator(const std::string& filePath) {
		asset = new Image();
		asset->filePath = filePath;
		asset->fromFile = true;
	}


	/**
	 * @brief	Constructs an Image from a data source. The image will copy the data on creation, and unloading this image will have no effect on memory.
	*/
	Image::Creator::Creator(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels, unsigned int rowAlignment) {
		asset = new Image();
		asset->fromFile = false;
		asset->width = width;
		asset->height = height;
		asset->channels = channels;
		asset->rowAlignment = rowAlignment;
		this->imageData = data;
	}


	void Image::Creator::onFinish() {
		if( !asset->fromFile ) {
			// The image should always be "loaded" when not from file
			// so we construct it here
			asset->createGLTexture(imageData);
		}
	}


	Image::Creator& Image::Creator::setPartiallyTransparent(bool toggle) {
		asset->partiallyTransparent = toggle;
		return *this;
	}


	Image::Creator& Image::Creator::setScaleMode(ScaleMode mode) {
		asset->scaleDownMode = mode;
		asset->scaleUpMode = mode;
		return *this;
	}


	Image::Creator& Image::Creator::setScaleMode(ScaleMode scaleDownMode, ScaleMode scaleUpMode) {
		asset->scaleDownMode = scaleDownMode;
		asset->scaleUpMode = scaleUpMode;
		return *this;
	}


	Image::Creator& Image::Creator::setWrapMode(WrapMode mode) {
		asset->wrapModeHorizontal = mode;
		asset->wrapModeVertical = mode;
		return *this;
	}


	Image::Creator& Image::Creator::setWrapMode(WrapMode horizontalMode, WrapMode verticalMode) {
		asset->wrapModeHorizontal = horizontalMode;
		asset->wrapModeVertical = verticalMode;
		return *this;
	}


	Image::Creator& Image::Creator::setMipmapMode(MipmapMode mode) {
		asset->mipmapMode = mode;
		return *this;
	}

	
	Image::Creator::operator Image* () {
		return finish();
	}


}