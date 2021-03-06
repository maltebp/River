#include "River/pch.h"

#include "AssetCollection.h"

namespace River {

	void AssetCollection::add(Asset* asset) {
		auto iterator = std::find(assets.begin(), assets.end(), asset);
		if( iterator == assets.end() )
			assets.push_back(asset);
	}

	void AssetCollection::load() {
		if( loaded ) return; // TODO: Add warning here
		for( auto asset : assets )
			asset->load();
		loaded = true;
	}

	void AssetCollection::unload() {
		if( !loaded ) return; // TODO: Add warning here
		for( auto asset : assets )
			asset->unload();
		loaded = false;
	}

	bool AssetCollection::isLoaded() {
		return loaded;
	}

}