#pragma once

#include "Game.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include "Audio/AL.h"
#include "Audio/AudioPlayer.h"

#include "Event/MouseEvent/MouseEventController.h"


namespace River {
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Utility functions / variables

	static int glfwErrorCode = 0;
	static std::string glfwErrorMsg;


	static void glfwErrorCallback(int errCode, const char* errStr) {
		glfwErrorCode = errCode;
		glfwErrorMsg = std::string(errStr);
	}


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Game

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

			// Fire Key Events 
			auto keyEvents = Window::getKeyEvents();
			for( auto& keyEvent : keyEvents ) {
				rootLayer->keyEvent(keyEvent);
			}

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