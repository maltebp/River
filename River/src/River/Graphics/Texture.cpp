#include "Texture.h"

#include <stb_image.h>

#include "Graphics.h"

#include "River/Error.h"


River::Texture::Texture(std::string filePath) {

	this->filePath = filePath;
	
	// Load image file
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	if (!data) {
		throw new River::AssetNotFoundException(filePath);
	}
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// Texture filtering/wrap options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture and free memory
	glBindTexture(1, 0);
	stbi_image_free(data);

	// Bind texture to slot: glBindVertexArray(slotIndex, textureId)
}


River::Texture::~Texture() {

	glBindTexture(1, 0);
	glDeleteTextures(1, &id);

	// TODO: Release the texture here
}
