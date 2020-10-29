#pragma once


namespace River {

	class AssetCollection;

	class Asset {
		friend class AssetCollection;
	public:


		/**
		 * @brief	Loads the asset from disk onto the heap
		*/
		virtual void load() = 0;
		
		/**
		 * @brief	Unloads the asset from the heap, freeing all memory it has used
		*/
		virtual void unload() = 0;
	
		/**
		 * @return	True if this asset has been successfully loaded onto the heap, and is
		 *			ready to be used.
		*/
		bool isLoaded() { return loaded; };

		/**
		 * @return	Pointer to the collection this asset belongs to, or nullptr if it doesn't belong to any collection 
		*/
		AssetCollection* getCollection() { return collection; }


	protected:
		bool loaded = false;
		AssetCollection* collection = nullptr;	
	};

}


