#pragma once


#include <stdexcept>
#include "Error.h"
#include "Graphics/Window.h"

namespace River {
	class Game {
		

	private:
		std::string title;
		
	protected:
		River::Window* window;

	public:
		Game(std::string title);
		virtual ~Game();

		/**
			@throws River::Exception Thrown if an error occurs in the game loop
		*/
		void start();



		static void Main() {

		}

		virtual void onInitialization() {}
		
		virtual void onUpdate() {}

		Window* getWindow();
	};
}
