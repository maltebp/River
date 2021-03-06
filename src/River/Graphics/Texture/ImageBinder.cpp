#include "River/pch.h"

#include "ImageBinder.h"

#include <iostream>
#include <string>

namespace River{

	ImageBinder::ImageBinder(unsigned int numSlots){
		this->numSlots = numSlots;

		textures = (Image**) malloc(sizeof(Image*) * numSlots);
	}
	
	ImageBinder::~ImageBinder(){
		free(textures);
	}

	unsigned int ImageBinder::addImage(Image* image){
		for( unsigned int i = 0; i < numTextures; i++ )
			if( textures[i] == image )
				return i;

		if( numTextures == numSlots )
			throw new NoTextureSlotException();

		textures[numTextures] = image;
		numTextures++;
		return numTextures - 1;
	}


	bool ImageBinder::isFull(){
		return numTextures == numSlots;
	}


	void ImageBinder::bind(ShaderProgram* shaderProgram){
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

	void ImageBinder::clear(){
		numTextures = 0;
	}

	int ImageBinder::getNumTextures(){
		return numTextures;
	}

	int ImageBinder::getNumSlots(){
		return numSlots;
	}

}