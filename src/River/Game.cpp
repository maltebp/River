#pragma once

#include "Game.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include "Graphics/GL.h"
#include "Audio/AL.h"
#include "Audio/AudioPlayer.h"
#include "Keyboard/KeyboardController.h"
#include "Event/MouseEvent/MouseEventController.h"


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

		std::cout << "GLEW initialized" << std::endl;
		std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "Graphics card: " << glGetString(GL_RENDERER) << std::endl;

		// Initialize audio system
		AL::initialize();

		Window::setClearColor(Colors::BLACK);

		if( onStart != nullptr ) {
			printf("Running on start callback\n");	
			onStart();
		}

		printf("Starting game loop\n");
		while( !Window::shouldClose() ) {
			Window::clear();

			rootLayer->clean();

			Window::invokeEvents();
			KeyboardController::invokeEvents();

			// Fire Mouse Movement Events
			if( MouseEventController::hasMovementOccured() ) {
				auto e = MouseEventController::getMouseMoveEvent();
				rootLayer->mouseMoveEvent(e);
			}

			// Fire Mouse Scroll Events
			if( MouseEventController::hasScrollingOccured() ) {
				auto e = MouseEventController::getMouseScrollEvent();
				rootLayer->mouseScrollEvent(e);
			}

			// Fire Mouse Button Events
			for( auto& buttonEvent : MouseEventController::getMouseButtonEvents() )
				rootLayer->mouseButtonEvent(buttonEvent);

			// TODO: Use correct time here
			AudioPlayer::updatePlayers(0.0166666666666);

			rootLayer->update();
		}

		printf("Game loop stopped\n");
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