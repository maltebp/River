#pragma once

namespace River {

	class AssetCollection;

	/**
	 * @brief	Abstract class which defines some resource in the system, which may be loaded and unloaded from memory.
     *			
	 * @details	Multiple sources may load and unload the asset, but it won't actually be unloaded before the load count
	 *			has reached zero. This allows for asset sharing, but it's not robust, since one source may unload multiple
	 *			times by accident, and cause other sources to try and access an unloaded asset.
	 *			
	*/
	class Asset {
		friend class AssetCollection;
	public:

		/**
		 * @brief	Increment the load count of this asset. If the load count was 0 the asset will be loaded by having its onLoad method called
		*/
		void load();

		/**
		 * @brief	Decrement the load count of this asset. If the load count becomes 0 the asset will be unloaded by having its onUnload method called
		*/
		void unload();

		/**
		 * @return	True if this asset has been successfully loaded onto the heap, and is
		 *			ready to be used.
		*/
		bool isLoaded();


	protected:

		/**
		 * @brief	Loads the asset, possibly allocating memory for it.
		*/
		virtual void onLoad() = 0;

		/**
		 * @brief	Unloads the asset, possibly deallocating memory for it.
		*/
		virtual void onUnload() = 0;


	protected:

		unsigned int loadCount = 0;
		
	};

}


