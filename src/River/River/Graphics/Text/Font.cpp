#include "River/pch.h"

#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace River {

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Freetype

	namespace Freetype {
		static FT_Library library;
		static bool initialized = false;

		void initialize() {
			if( initialized ) return;

			if( FT_Init_FreeType(&library) ) {
				// TODO: Proper exception here
				throw River::Exception("Could not initialize FreeType library");
			}

			initialized = true;
		}
	}
	//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


	Font::Font(const std::string& fontPath) {
		path = fontPath;
	}

	
	FontInstance* Font::getFontInstance(unsigned int size) {
		if( !isLoaded() )
			throw new AssetNotLoaded("The font '" + path + "' has not been loaded");
	
		auto it = instanceMap.find(size);
		if( it != instanceMap.end() )
			return it->second;

		if( !autoLoadSizes )
			throw new AssetNotLoaded("Font size " + std::to_string(size) + " has not been loaded, and auto loaded is not enabled");

		auto result = instanceMap.emplace(size, new FontInstance(size, nativeFontType));
		return result.first->second;
	}


	void Font::onLoad() {
		Freetype::initialize();

		// Initialize the font
		FT_Error result = FT_New_Face(Freetype::library, path.c_str(), 0, (FT_Face*)&nativeFontType);
		if( result > 0 ) {
			// TODO: Implement proper exception
			throw River::AssetException("Error when loading Font '" + path + "' (Freetype error: " + std::to_string(result) + ")");
		}

		// Preload font sizes
		for( auto size : sizesToPreload ) {
			auto result = instanceMap.emplace(size, new FontInstance(size, nativeFontType));
		}	
	}


	void Font::onUnload() {
		// Deleting size instances
		for( auto pair : instanceMap ) 
			pair.second->destroy();
		instanceMap.clear();

		// Remove face
		FT_Done_Face((FT_Face) nativeFontType);
	}



	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Creator

	Font::Creator::Creator(const std::string& fontPath) {
		asset = new Font(fontPath);
	}


	Font::Creator& Font::Creator::enableSizeAutoLoading() {
		asset->autoLoadSizes = true;
		return *this;
	}


	Font::Creator& Font::Creator::preloadSize(unsigned int fontSize) {
		asset->sizesToPreload.insert(fontSize);
		return *this;
	}


	Font::Creator::operator Font*() {
		return finish();
	}

}