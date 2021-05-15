#pragma once

#include <string>

#include "Image.h"
#include "../Shader/ShaderProgram.h"
#include "River/Error.h"

namespace River {


	class ImageBinder {
	public:

		/**
		 * @param numSlots	Number of slots the ImageBinder images  to
		*/
		ImageBinder(unsigned int numSlots);

		~ImageBinder();

		/**
		 * @brief	Add an Image to be bound, when this binder's bind(..) method is called.
		 *
		 * @return	The texture slot index, in which the texture will be placed in, or already has been placed in.
		 * 			If negative, no texture slot was available
		*/
		int addImage(Image* texture);

		/**
		 * @brief	Binds the texture to a given ShaderProgram. The program must be bound beforehand
		*/
		void bind(ShaderProgram* shaderProgram, const std::string& uniformName);

		void clear();

		int getNumTextures();

		int getNumSlots();


	private:

		Image** textures;

		unsigned int numTextures = 0; // White is first texture

		unsigned int numSlots = 0;

	};

}


