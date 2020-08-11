#include "Texture.h"

#include <River/Vendor/stb_image/stb_image.h>
#include "GL.h"
#include "River/Error.h"

namespace River {

	Texture* Texture::whiteTexture = nullptr;

	Texture::Texture(std::string filePath){
		this->filePath = filePath;

		// Load image file
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		if( !data ){
			throw new River::AssetNotFoundException(filePath);
		}

		GL(glGenTextures(1, &id));
		GL(glBindTexture(GL_TEXTURE_2D, id));

		// Texture filtering/wrap options
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));

		// TODO: Re-add mipmap
		//glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture and free memory
		// TODO: Rebind previously bound texture
		GL(glBindTexture(GL_TEXTURE_2D, 0));
		stbi_image_free(data);
	}

	Texture::Texture(unsigned int width, unsigned int height, void* data){
		this->width = width; 
		this->height = height;
		this->channels = 4;

		GL(glGenTextures(1, &id));
		GL(glBindTexture(GL_TEXTURE_2D, id));
		// Texture filtering/wrap options
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

		// TODO: Re-add mipmap
		//glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture and free memory
		GL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	Texture::~Texture(){
		if( glIsTexture(id) == GL_TRUE )
			GL(glDeleteTextures(1, &id));
		// Automatically unbinds the texture as well
	}
		
	void Texture::bind(unsigned int textureSlot){
		// Note: Not using glBindTextureUnit as this is only available from 4.5 and onwards
		GL(
			glActiveTexture(GL_TEXTURE0+textureSlot);
			glBindTexture(GL_TEXTURE_2D, id);
		);
	}


	Texture* Texture::getWhiteTexture(){
		if( whiteTexture == nullptr ){
			uint32_t data = 0xffffffff;
			whiteTexture = new Texture(1, 1, &data);
		}
		return whiteTexture;
	}
}