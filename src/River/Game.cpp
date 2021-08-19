#include "River/pch.h"

#include "Game.h"

#include "Graphics/GL.h"
#include "River/External/imgui/imgui.h"
#include "Audio/AL.h"
#include "Audio/AudioPlayer.h"
#include "Graphics/FrameBuffer.h"
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

		FrameBuffer* frameBuffer = nullptr;

		if( onStart != nullptr ) {
			onStart();
		}

		while( !Window::shouldClose() ) {
			Resolution sceneViewSize = {500, 500};
			bool sceneViewSizeChanged = true;

			Window::update(Game::update, Game::updateImGui);
		}

	}


	void Game::update() {
		// Note on events:
		// A button press from the mouse is not registered as pressed
		// in the same event cycle as a Keyboard event. However, it
		// will be available in the next cycle, and for now it shouldn't
		// cause a problem

		rootLayer->clean();
			
		Window::invokeEvents();

		// TODO: Use correct time here
		AudioPlayer::updatePlayers(0.0166666666666);		

		KeyboardController::invokeEvents();

		MouseController::invokeEvents();

		rootLayer->update();	
	}


	void Game::updateImGui() {
		rootLayer->updateImGui();
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

	
	void Game::enableImGui() {
		if( started ) {
			throw new InvalidStateException("Cannot enable editor mode when game is already started");
		}
		
		imGuiEnabled = true;
	}


	bool Game::isImGuiEnabled() {
		return imGuiEnabled;
	}

	
}