#pragma once

#include <vector>
#include <functional>

#include "Error.h"
#include "Graphics/Window.h"
#include "Layer/Layer.h"
#include "Primitives/Color.h"
#include "Asset/AssetCollection.h"

namespace River {

	class Game {
	public:
	
		static void start();
		
		static void start(std::function<void()> onStart);

		static void exit();
		
		static Layer* pushLayer() {
			return rootLayer->pushLayer();
		}

		template <typename L, typename ... Args>
		static L* pushLayer(Args ... args) {
			return rootLayer->pushLayer<L>(args...);
		}

		static void clearLayers();

		static void removeLayer(Layer* layer);


	public:

	private:


		static inline std::function<void()> initCallback = nullptr;

		static inline bool started = false;
		
		/**
		 * @brief	An "abstract" layer in that it has no callback, only serves as a container for sub layers and will never be deleted
		*/
		static inline Layer* rootLayer = new Layer();




	};
}
