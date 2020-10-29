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
		 * @tparam A	Any class inherting from the River::Asset class 
		 * @param a		The asset to add. Does nothing if the asset has already been added
		 * @return		The same pointer which was passed this function (allows for creation of assets and adding them to the collection in one line).
		*/
		template <class A>
		A* add(A* a) {
			static_assert(std::is_convertible<A*, Asset*>::value, "Class type must inherit publicly from River::Asset");
			
			auto casted = (Asset*)a;
			if( casted->collection != nullptr )
				throw new InvalidStateException("Asset is already in a collection");

			auto iterator = std::find(assets.begin(), assets.end(), a);
			if( iterator == assets.end() )
				assets.push_back(a);

			casted->collection = this;

			return a;
		}



	private:
		std::vector<Asset*> assets;
	};
}



