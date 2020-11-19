#include "Asset.h"

namespace River {

	void Asset::load() {
		loadCount++;
		if( loadCount == 1 )
			onLoad();
	}


	void Asset::unload() {
		if( loadCount == 0 ) return;

		loadCount--;
		if( loadCount == 0 )
			onUnload();
	}


	bool Asset::isLoaded() {
		return loadCount > 0;
	}

}