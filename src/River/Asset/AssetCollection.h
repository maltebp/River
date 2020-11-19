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

		/**
		 * @brief Adds an asset to this collection
		 *
		 * @param a		The asset to add. Does nothing if the asset has already been added
		*/
		void add(Asset* a);

		/**
		 * @brief	Loads all the Assets in this collection. While some of the Assets may already be
		 *			loaded, this ensures they stay loaded as long as this collection is loaded.
		 *			Logs a warning if the collection is not already loaded.
		*/
		void load();

		/**
		 * @brief	Unloads all the Assets in this collection. Some Assets may remain loaded, if they
		 *			have been loaded by any other collection.
		 *			Logs a warning if the collection is not already loaded.
		*/
		void unload();

		/**
		 * @return	Whether or not this asset is loaded
		*/
		bool isLoaded();

	private:
		bool loaded = false;
		std::vector<Asset*> assets;
	};
}



