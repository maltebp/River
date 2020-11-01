#pragma once

#include <string>

#include "River/Asset/AssetCollection.h"
#include "Image.h"

namespace River {


	/**
	 * @brief	A Texture defines a drawable part of some Image. This can be the enitre Image or just a specified region of it.
	*/
	class Texture : public Asset {
	public:


		/**
		 * @brief The non-normalized Texture width
		*/
		unsigned int getWidth();

		/**
		 * @brief The non-normalized Texture width
		*/
		unsigned int getHeight();
		

		/**
		 * @return	The aspectio ratio of the width and height, calculated by dividing width with height.
		*/
		double getAspectRatio();

		Image* getImage() const;


		void flipVertically();

		void flipHorizontally();

		/**
		 * @brief 
		 
		*/
		//void rotate(int times);

		const Image::SampleCoordinates& getTextureCoordinates() const;

		/**
		 * @brief	Whether or not this Texture's texture is only used by this Texture. If so, the texture will be deleted
					with this Texture
		*/
		bool hasDedicatedImage() const;


		/**
		 * @brief	Checks whether or not this texture contains partially transparent pixels
		*/
		bool isPartiallyTransparent() const;


		virtual void onLoad() override;
		virtual void onUnload() override;


	private:
		Texture();
		~Texture();
		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture&) = delete;

		void updateAdjustedCoordinates();


	private:
		Image* image;
		bool dedicatedImage = false;

		Image::SampleCoordinates textureCoordinates;
		Image::SampleCoordinates flippedCoordinates;

		// Whether or not this texture contains partially transparent areas
		bool partiallyTransparent;

		unsigned int width = 0;
		unsigned int height = 0;

		bool verticallyFlipped = false;
		bool horizontallyFlipped = false;
		


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Creator
	public:
		class Creator {
			friend class Texture;

			Texture* texture;

			AssetCollection* collection = nullptr;

			Creator(Image* image, bool dedicatedImage);

		public:

			/**
			 * @brief	Sets the offset for sampling the image
			*/
			Creator& setSamplingOffset(unsigned int x, unsigned int y);

			/**
			 * @brief	Sets the size for sampling the image
			*/
			Creator& setSamplingSize(unsigned int width, unsigned height);

			/**
			 * @brief	Sets that this Texture should be drawn as partially transparent. Only partially transparent textures
			 *			will get partially transparent pixels drawn.
			*/
			Creator& setPartiallyTransparent();

			/**
			 * @brief	Finishes the creation of the Texture
			 * @return	Pointer to the dynamically allocated Texture
			*/
			Texture* finish();

			Creator& setAssetCollection(AssetCollection*);

		};


		/**
		 * @brief	Creates a Texture from a shared Image. The image will be loaded/unloaded together with the Texture.
		 * @param source	The source image this Texture should be sampled from
		 * @param dedicatedSource	Whether or not this Texture should take ownership of the source. If true, then the Image will be destroyed
		 *							together with the Texture, and the pointer should be passed only to this method.
		*/
		static Creator create(Image* source, bool dedicatedSource) { return Creator(source, dedicatedSource); }

	};




}


