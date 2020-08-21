#pragma once

#include <string>

#include "Texture.h"
#include "../Shader/ShaderProgram.h"
#include "River/Error.h"

namespace River{

	class TextureBinder{
	
	private:
		Texture** textures;
		unsigned int numTextures = 0; // White is first texture
		unsigned int numSlots = 0;
		std::string uniformSamplerName = "u_Textures";
		
	public:
		/**
		 * @param numSlots	Number of slots the TextureBinder may bind textures to
		*/
		TextureBinder(unsigned int numSlots);
		~TextureBinder();

		/**
		 * @brief	Adds the texture to be bound in a given texture slot (use isFull() to check if any slots are available).
					The function does nothing if the texture is already bound
		 * @return	The texture slot index, in which the texture will be placed in, or already has been placed in
		 * @throws  River::Exception	Thrown if there is no slots available
		*/
		unsigned int addTexture(Texture* texture);

		/**
		 * @brief	Binds the textures to a given ShaderProgram. The program must be bound beforehand
		*/
		void bind(ShaderProgram* shaderProgram);

		bool isFull();

		void clear();	

		int getNumTextures();
		int getNumSlots();
	};

}


