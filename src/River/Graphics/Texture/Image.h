#pragma once

#include <string>


#include "River/Asset/AssetCollection.h"
#include "../GL.h"

namespace River {

	class Image : public Asset {
	public:

		struct SampleCoordinates {
			float x1; // Left
			float y1; // Top
			float x2; // Right
			float y2; // Bottom
		};


		enum class ScaleMode {
			LINEAR, NEAREST
		};

	public:
		


		void bind(unsigned int textureSlot);
		unsigned int getId() { return id; }

		unsigned int getNumChannels();

		static Image *getWhiteTexture();

		unsigned int getWidth();
		unsigned int getHeight();

		

		/**
		 * @brief Normalizes the x-coordinate into the OpenGL coordinates (ranging from 0 to 1);
		*/
		float normalizeX(unsigned int coordinate);


		/**
		 * @brief Normalizes the x-coordinate into the OpenGL coordinates (ranging from 0 to 1);
		*/
		float normalizeY(unsigned int coordinate);


		virtual void load() override;
		virtual void unload() override;



	private:
		Image(){}
		~Image(){}

		void createGLTexture(void* data);

		// Prevent copying and assignemnt 
		Image(const Image& temp_obj) = delete;
		Image& operator=(const Image& temp_obj) = delete;


	private:
		static inline Image* whiteTexture = nullptr;

		// OpenGL texture id
		unsigned int id;
		std::string filePath = "";

		int width;
		int height;
		int channels;
		int rowAlignment;

		/**
		 * @brief Only used if the image is not loaded from the disk
		*/
		unsigned char* pixels = nullptr;

		/**
		 * @brief	Whether or not this texture contains any partially transparent pixels. Curently, it's up to the user to
		 *			decide when constructing an texture
		*/
		bool partiallyTransparent;

		ScaleMode scaleMode = ScaleMode::LINEAR;




	public:

		class Creator {
			friend class Image;

		private:
			Creator(const std::string& filePath);
			Creator(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels, unsigned int rowAlignment);
		
		public:
			Image* finish();

			Creator& setAssetCollection(AssetCollection*);
			Creator& setPartiallyTransparent(bool toggle);
			Creator& setScaleMode(ScaleMode mode);


		private:
			Image* image;
			AssetCollection* assetCollection = nullptr;
		};
		

		static Creator create(const std::string& filePath) {
			return Creator(filePath);
		}

		static Creator create(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels, unsigned int rowAlignment) {
			return Creator(data, width, height, channels, rowAlignment);
		}
	};


}


