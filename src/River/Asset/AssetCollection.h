#pragma once

#include <type_traits>
#include <vector>

#include "River/Error.h"
#include "River/Asset/Asset.h"

namespace River {


	/**
	 * @brief	Defines a collection of assets, which can be loaded and unloaded collectively
	*/
	class AssetCollection {
	public:

		void load();

		void unload();

		/**
		 * @brief Adds an asset to this collection
		 *
		 * @param a		The asset to add. Does nothing if the asset has already been added
		*/
		void add(Asset* a) {
			auto iterator = std::find(assets.begin(), assets.end(), a);
			if( iterator == assets.end() )
				assets.push_back(a);
		}



	private:
		std::vector<Asset*> assets;
	};
}



