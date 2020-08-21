#pragma once

#include <string>

#include "Font.h"
#include "FontType.h"


namespace River {

	class FontController {
	private:
		static bool initialized;
		static void* nativeLibrary;
		static std::string fontFolder;
		static std::unordered_map<std::string, FontType*> fontTypeMap;

	public:
		static void initialize();
		static void setFontFolder(const std::string &fontFolder);
		static Font* getFont(const std::string &fontName, unsigned int size);
		
	};
}


