#pragma once

#include <string>
#include <unordered_map>

#include "Font.h"

#include "River/Error.h"

namespace River {

	class FontType {
		void *nativeFontType;

		std::unordered_map<unsigned int, Font*> fontMap;
		const std::string name;

	public:
		FontType(const std::string &name, const std::string &path, void *nativeLibrary);

		Font* getFont(unsigned int size);

		const std::string& getName() const;
	
	private:
		Font* createFont(unsigned int size);
	};

}


