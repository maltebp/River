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

	int ImageBinder::addImage(Image* image){
		for( unsigned int i = 0; i < numTextures; i++ ) {
			if( textures[i] == image ) return (int)i;
		}

		if( numTextures == numSlots ) return -1;

		textures[numTextures] = image;
		numTextures++;
		return numTextures - 1;
	}


	void ImageBinder::bind(ShaderProgram* shaderProgram, const std::string& uniformName){
		if( numTextures == 0 ) return;

		if( !shaderProgram->hasUniform(uniformName) ){
			throw River::InvalidArgumentException("Shader program does not have the uniform '" + uniformName + "' when binding textures");
		}

		std::vector<int> textureSamples(numTextures);
		for( unsigned int i = 0; i < numTextures; i++ ){
			textureSamples[i] = (int) i;
		}

		shaderProgram->setIntArray(uniformName, textureSamples.data(), numTextures);

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