#include "TextureBinder.h"

#include <iostream>
#include <string>

namespace River{

	TextureBinder::TextureBinder(unsigned int numSlots, bool addWhiteTexture){
		this->numSlots = numSlots;
		this->addWhiteTexture = addWhiteTexture;

		textures = (Texture**) malloc(sizeof(Texture*) * numSlots);
		
		if( addWhiteTexture ){
			numTextures = 1;
			textures[0] = Texture::getWhiteTexture();
		}
	}
	
	TextureBinder::~TextureBinder(){
		free(textures);
	}

	unsigned int TextureBinder::addTexture(Texture* texture){
		for( unsigned int i = 0; i < numTextures; i++ )
			if( textures[i] == texture )
				return i;

		if( numTextures == numSlots )
			throw new NoTextureSlotException();

		textures[numTextures] = texture;
		numTextures++;
		return numTextures - 1;
	}


	bool TextureBinder::isFull(){
		return numTextures == numSlots;
	}


	void TextureBinder::bind(ShaderProgram* shaderProgram){
		if( numTextures == 0 ) return;

		if( !shaderProgram->hasUniform(uniformSamplerName) ){
			// TODO: Create proper logging here (and remove iostream)
			std::cout << "WARNING: ShaderProgram does not have the uniform '" << uniformSamplerName << "' when binding textures!" << std::endl;
			return;
		}

		std::vector<int> textureSamples(numTextures);
		for( unsigned int i = 0; i < numTextures; i++ ){
			textureSamples[i] = (int) i;
		}
		shaderProgram->setIntArray(uniformSamplerName, textureSamples.data(), numTextures);

		for( unsigned int j = 0; j < numTextures; j++ ){
			textures[j]->bind(j);
		}
	}

	void TextureBinder::clear(){
		if( addWhiteTexture ) numTextures = 1;
		else numTextures = 0;
	}

	int TextureBinder::getNumTextures(){
		return numTextures;
	}

	int TextureBinder::getNumSlots(){
		return numSlots;
	}

}