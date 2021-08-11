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
	

		//std::cout << "GLEW initialized" << std::endl;
		//std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
		//std::cout << "Graphics card: " << glGetString(GL_RENDERER) << std::endl;

		// Initialize audio system
		AL::initialize();

		if( onStart != nullptr ) {
			onStart();
		}

		while( !Window::shouldClose() ) {
  
			Window::beginFrame();

			rootLayer->clean();

			// Note on events:
			// A button press from the mouse is not registered as pressed
			// in the same event cycle as a Keyboard event. However, it
			// will be available in the next cycle, and for now it shouldn't
			// cause a problem

			Window::invokeEvents();

			// TODO: Use correct time here
			AudioPlayer::updatePlayers(0.0166666666666);		

			KeyboardController::invokeEvents();

			MouseController::invokeEvents();

			rootLayer->update();

			Window::endFrame();

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

	
	void Game::enableEditorMode() {
		if( started ) {
			throw new InvalidStateException("Cannot enable editor mode when game is already started");
		}
		
		editorMode = true;
	}

	bool Game::isInEditorMode() {
		return editorMode;
	}
	
}