#include "River/pch.h"

#include "Game.h"

#include "Graphics/GL.h"
#include "Audio/AL.h"
#include "Audio/AudioPlayer.h"
#include "Keyboard/KeyboardController.h"
#include "River/Mouse/MouseController.h"


namespace River {


	void Game::start() {
		Game::start(nullptr);
	}


	void Game::start(std::function<void()> onStart) {

		if( started )
			throw new InvalidStateException("Game has already been started");

		Window::open();

		// Initialize glew
		const GLenum glewResult = glewInit();
		if( glewResult != GLEW_OK ) {
			std::stringstream msgStream;
			msgStream << "GLEW initialization error '" << glewGetErrorString(glewResult) << "'";
			throw Exception(msgStream.str());
		}

		//std::cout << "GLEW initialized" << std::endl;
		//std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
		//std::cout << "Graphics card: " << glGetString(GL_RENDERER) << std::endl;

		// Initialize audio system
		AL::initialize();

		Window::setClearColor(Colors::BLACK);

		if( onStart != nullptr ) {
			onStart();
		}

		while( !Window::shouldClose() ) {

			Window::clear();

			rootLayer->clean();

			// TODO: Use correct time here
			AudioPlayer::updatePlayers(0.0166666666666);

			// Note on events:
			// A button press from the mouse is not registered as pressed
			// in the same event cycle as a Keyboard event. However, it
			// will be available in the next cycle, and for now it shouldn't
			// cause a problem

			Window::invokeEvents();

			KeyboardController::invokeEvents();

			MouseController::invokeEvents();

			rootLayer->update();
		}

	}


	void Game::removeLayer(Layer* layer) {
		rootLayer->removeLayer(layer);
	}


	void Game::clearLayers() {
		rootLayer->clearLayers();
	}


	void Game::exit() {
		Window::close();
	}
	
}