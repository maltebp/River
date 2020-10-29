#include "AssetCollection.h"

namespace River {

	void AssetCollection::load() {
		for( auto asset : assets )
			asset->load();
	}

	void AssetCollection::unload() {
		for( auto asset : assets )
			asset->unload();
	}

}