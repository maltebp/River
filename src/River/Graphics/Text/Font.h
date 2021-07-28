#pragma once

#include <string>
#include <unordered_map>
#include <set>

#include "River/Asset/Asset.h"
#include "River/Asset/AssetCreator.h"

#include "FontInstance.h"

#include "River/Error.h"

namespace River {

	class Font : public Asset {
	public:

		virtual void onLoad() override;
		virtual void onUnload() override;
	
	private:
		Font(const std::string& fontPath);
		FontInstance* getFontInstance(unsigned int size);


	private:
		void* nativeFontType;

		std::unordered_map<unsigned int, FontInstance*> instanceMap;

		std::string path = "";

		bool autoLoadSizes = false;

		std::set<unsigned int> sizesToPreload;


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Creator

	public:
		class Creator : public AssetCreator<Creator, Font> {
			friend class Font;

		public:

			/**
			 * @brief	Automatically load a new Font size, if it doesn't exist and its
			 *			being requested.
			*/
			Creator& enableSizeAutoLoading();

			/**
			 * @brief	Load the particular Font size, when loading the Font
			*/
			Creator& preloadSize(unsigned int fontSize);

			operator Font*();

		private:

			Creator(const std::string& fontPath);

		};

	
	static Creator create(const std::string& fontPath){
		return Creator(fontPath);
	}

	friend class Creator;
	friend class TextRenderingSystem;

	};

}


