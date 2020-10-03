#include "Texture.h"

#include <River/Vendor/stb_image/stb_image.h>
#include "../GL.h"
#include "River/Error.h"

namespace River {

	Texture *Texture::whiteTexture = nullptr;

	Texture::Texture(std::string filePath) {
		this->filePath = filePath;

		// TODO: Determine the correct alignment from the image
		rowAlignment = 4;

		// Load image file
		//stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		if( !data ) {
			throw new River::AssetNotFoundException(filePath);
		}

		createGLTexture(data);
		stbi_image_free(data);
	}


	Texture::Texture(unsigned int width, unsigned int height, unsigned int channels, unsigned int rowAlignment, void *data) {
		this->width = width;
		this->height = height;
		this->channels = channels;
		this->rowAlignment = rowAlignment;

		createGLTexture(data);
	}

	Texture::~Texture() {
		if( glIsTexture(id) == GL_TRUE )
			GL(glDeleteTextures(1, &id));
		// Automatically unbinds the texture as well
	}


	void Texture::createGLTexture(void *data) {

		GL(glPixelStorei(GL_UNPACK_ALIGNMENT, rowAlignment));

		GL(glGenTextures(1, &id));
		GL(glBindTexture(GL_TEXTURE_2D, id));

		// Texture filtering/wrap options
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); // GL_LINEAR_MIPMAP_LINEAR));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

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


	void Texture::bind(unsigned int textureSlot) {
		// Note: Not using glBindTextureUnit as this is only available from 4.5 and onwards
		GL(
			glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, id);
		);
	}



	void Texture::setFilterMode(FilterMode mode) {
		GLint currentTexture = 0;
		GL(glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture));

		GL(glBindTexture(GL_TEXTURE_2D, id));
		if( mode == FilterMode::LINEAR ) {
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		}
		if( mode == FilterMode::NEAREST ) {
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		}

		GL(glBindTexture(GL_TEXTURE_2D, currentTexture));
	}
	

	unsigned int Texture::getNumChannels() {
		return channels;
	}

	unsigned int Texture::getWidth() {
		return width;
	}

	unsigned int Texture::getHeight() {
		return height;
	}

	Texture *Texture::getWhiteTexture() {
		if( whiteTexture == nullptr ) {
			unsigned char data = 0xFF;
			whiteTexture = new Texture(1, 1, 1, 1, &data);
		}
		return whiteTexture;
	}

	float Texture::normalizeX(unsigned int coordinate) {
		return (float) coordinate / (float) width;
	}

	float Texture::normalizeY(unsigned int coordinate) {
		return (float) coordinate / (float) height;
	}
}