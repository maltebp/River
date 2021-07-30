#pragma once

#include <string>

#include "River/Asset/AssetCreator.h"
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
			LINEAR,
			NEAREST
		};


		enum class MipmapMode {
			NONE,
			LINEAR,
			NEAREST
		};

		
		/**
		 * @brief	How the image should be sampled, when sampling
		 * 			outside the texture coordinates bounds. Matches
		 * 			the OpenGL "wrap" modes
		 */
		enum class WrapMode {

			/**
			 * @brief	Sample black color (GL_CLAMP_TO_BORDER)
			 */
			NONE,	

			/**
			 * @brief	Repeats the image (GL_REPEAT)
			 */
			REPEAT,

			/**
			 * @brief	Repeat the closest pixel (GL_CLAMP_TO_EDGE)
			 */
			CLAMP
		};

	public:

		void bind(unsigned int textureSlot);

		unsigned int getId() { return id; }

		unsigned int getNumChannels();

		static Image* getWhiteTexture();

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

		bool isPartiallyTransparent() const;

		static GLenum toGLScaleMode(ScaleMode scaleMode, MipmapMode mipmapMode = MipmapMode::NONE);

		static GLenum toGLWrapMode(WrapMode wrapMode);

		static GLenum toGLFormat(int channels);

	protected:

		/**
		 * @brief	Loads the image, if the image was constructed from a filepath
		*/
		virtual void onLoad() override;

		virtual void onUnload() override;

	private:

		Image();

		~Image();
		
		Image(const Image& temp_obj) = delete;

		Image& operator=(const Image& temp_obj) = delete;

		void destroy();

		void createGLTexture(void* data);

	private:

		static inline Image* whiteTexture = nullptr;

		// OpenGL texture id
		unsigned int id;

		std::string filePath = "";

		bool fromFile = false;

		int width;
		int height;

		int channels;

		int rowAlignment;

		/**
		 * @brief	Whether or not this texture contains any partially transparent pixels. Curently, it's up to the user to
		 *			decide when constructing an texture
		*/
		bool partiallyTransparent;

		ScaleMode scaleUpMode 		= ScaleMode::LINEAR;
		ScaleMode scaleDownMode 	= ScaleMode::LINEAR;

		WrapMode wrapModeHorizontal = WrapMode::NONE;
		WrapMode wrapModeVertical 	= WrapMode::NONE;

		MipmapMode mipmapMode = MipmapMode::LINEAR;

	public:

		class Creator : public AssetCreator<Creator, Image> {
			friend class Image;

		private:

			Creator(const std::string& filePath);

			Creator(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels, unsigned int rowAlignment);
		
		public:

			Creator& setPartiallyTransparent(bool toggle);

			Creator& setScaleMode(ScaleMode mode);

			Creator& setScaleMode(ScaleMode scaleDownMode, ScaleMode scaleUpMode);

			/**
			 * @brief	Set the horizontal and vertical wrapping mode of the image.
			 * 			The wrapping mode describes how the image should be sampled
			 * 			when sampled outside the texture coordinates bounds.
			 * 			The default for both vertical and horizontal is NONE.
			 */
			Creator& setWrapMode(WrapMode mode);

			/**
			 * @brief	Set the horizontal and vertical wrapping mode of the image.
			 * 			The wrapping mode describes how the image should be sampled
			 * 			when sampled outside the texture coordinates bounds.
			 * 			The default for both vertical and horizontal is NONE.
			 */
			Creator& setWrapMode(WrapMode horizontalMode, WrapMode verticalMode);

			/**
			 * @brief	Sets the mode for mipmaps when scaling down the texture.
			 * 			Default is LINEAR. Setting it to NONE will disables mipmaps
			 * 			for this Image
			 */
			Creator& setMipmapMode(MipmapMode mode);

			operator Image*();

		protected:

			void onFinish() override;	

		private:

			// Temporary pointer to image data, when not loading from file
			unsigned char* imageData = nullptr;

		};
		

		static Creator create(const std::string& filePath) {
			return Creator(filePath);
		}


		static Creator create(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels, unsigned int rowAlignment) {
			return Creator(data, width, height, channels, rowAlignment);
		}


		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// Friend classes

		friend class Texture; // For deleting

	};


}


