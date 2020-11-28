#pragma once

#include "Asset.h"
#include "AssetCollection.h"

namespace River {

	// Just there allow for template sample argument in Visual Studio
	class AssetSample : public Asset{};
	class CreatorSample;
	

	/**
	 * @brief	Class which supplies some generic methods for a builder-like pattern
	 *			for a type of Asset.
	 *
	 * @tparam C	The Creator type (to allow for correctly returning of the Creator in methods 
	 * @tparam A	The Asset type (must publicly inherit from River::Asset)
	*/
	template <typename C, typename A>
	class AssetCreator {
	public:

		static_assert(std::is_convertible<A*, Asset*>::value, "Class must inherit publicly from River::Asset");


		/**
		 * @brief	Add this Asset to an AssetCollection. If the asset is already added to the collection,
		 *			it will do nothing
		 * @param collection 
		 * @return 
		*/
		C& addToCollection(AssetCollection* collection) {
			collections.push_back(collection);
			return *((C*)this);
		}


		/**
		 * @brief	Finishes the construction of this Asset.
		 * @return	A pointer to the heap-allocated Asset.
		 *
		 * @throws	River::InvalidStateException	If the Asset has already been finished (this method has been called before)
		*/
		A* finish() {
			if( finished )
				throw new InvalidStateException("AssetCreator has already finished");

			for( auto collection : collections ) {
				collection->add(asset);
			}

			onFinish();

			finished = true;

			return asset;
		}

		

	protected:


		/**
		 * @brief	Method which is called right before the Asset returned, when finish is called
		*/
		virtual void onFinish() {}

		
		A* asset;

	private:
		std::vector<AssetCollection*> collections;
		bool finished = false;
	};

}
