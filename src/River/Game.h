#pragma once

#include <vector>
#include <functional>

#include "Error.h"
#include "Graphics/Window.h"
#include "Layer/SimpleLayer.h"
#include "Primitives/Color.h"
#include "Asset/AssetCollection.h"

namespace River {

	class Game {
	public:
	
		static void setTitle(const std::string&);

		static void setWindowSize(unsigned int width, unsigned int height);

		static void start();
		
		static void start(std::function<void()> onStart);

		static void exit();

		static double getFps();


		static Window* getWindow();


		template <typename L, typename ... Args>
		static L* pushLayer(Args ... args) {
			return rootLayer->pushLayer<L>(args...);
		}


		static void clearLayers();

		static void removeLayer(Layer* layer);

		/**
		 * @brief	Sets the color to clear the screen with. Default is Lawn Green.
		*/
		static void setClearColor(Color color);

		/**
		 * @brief	Sets the color to clear the screen with. Default is Lawn Green.
		*/
		static const Color& getClearColor();





	public:
		static inline Window* window = nullptr;

	private:


		static inline std::function<void()> initCallback = nullptr;

		static inline bool started = false;

		static inline std::string title = "River";

		static inline unsigned int windowWidth	= 1280;
		static inline unsigned int windowHeight = 720;


		static inline Color clearColor = Colors::LAWN_GREEN;

		/**
		 * @brief	An "abstract" layer in that it has no callback, only serves as a container for sub layers and will never be deleted
		*/
		static inline Layer* rootLayer = new SimpleLayer();




	};
}
