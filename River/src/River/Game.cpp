#pragma once

#include "Game.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

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

	static void printGLErrors() {
		GLenum err;
		while( (err = glGetError()) != GL_NO_ERROR ) {
			std::cout << "GL ERROR: " << std::hex << err << std::endl;
		}
	}



	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Game


	void Game::start() {
		Game::start(nullptr);
	}

	void Game::start(std::function<void()> onStart) {

		if( started )
			throw new InvalidStateException("Game has already been started");

		window = new Window(title, 1280, 720);

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


		if( onStart != nullptr ) {
			printf("Running on start callback\n");	
			onStart();
		}



		printf("Starting game loop\n");
		while( !window->shouldClose() ) {
			window->clear();

			for( auto layer : layersToAdd ) {
				layers.push_back(layer);
				layer->initialize();
			}
			layersToAdd.clear();

			for( auto layer : layersToRemove ) {
				auto iterator = std::find(layers.begin(), layers.end(), layer);
				layers.erase(iterator);
				layer->terminate();
				delete layer;
			}
			layersToRemove.clear();

			// Fire Key Events 
			auto keyEvents = window->getKeyEvents();
			for( auto& keyEvent : keyEvents ) {
				
				for( auto it = layers.rbegin(); it != layers.rend(); it++ ) {
					if( keyEvent.isConsumed() ) break;
					(*it)->onKeyEvent(keyEvent);
				}
			}

			// Fire Mouse Movement Events
			if( MouseEventController::hasMovementOccured() ) {
				auto mouseMoveEvent = MouseEventController::getMouseMoveEvent();
		
				for( auto it = layers.rbegin(); it != layers.rend(); it++ ) {
					if( mouseMoveEvent.isConsumed() ) break;
					(*it)->onMouseMoveEvent(mouseMoveEvent);
				}
			}

			// Fire Mouse Scroll Events
			if( MouseEventController::hasScrollingOccured() ) {
				auto mouseScrollEvent = MouseEventController::getMouseScrollEvent();

				for( auto it = layers.rbegin(); it != layers.rend(); it++ ) {
					if( mouseScrollEvent.isConsumed() ) break;
					(*it)->onMouseScrollEvent(mouseScrollEvent);
				}
			}

			// Fire Mouse Button Events
			for( auto& buttonEvent : MouseEventController::getMouseButtonEvents() ) {
				
				for( auto it = layers.rbegin(); it != layers.rend(); it++ ) {
					if( buttonEvent.isConsumed() ) break;
					(*it)->onMouseButtonEvent(buttonEvent);
				}
			}
			


			for( auto& layer : layers ) {
				layer->update();
				window->clearDepth();
			}

		}

		printf("Game loop stopped\n");
	}



	void Game::setTitle(const std::string& title) {
		Game::title = title;
	}


	void Game::setWindowSize(unsigned int width, unsigned int height) {
		if( started )
			throw new InvalidStateException("Game has already been started, so window size can't be changed");
		Game::windowWidth = width;
		Game::windowHeight = height;
	}


	Window* Game::getWindow() {
		return window;
	}


	double Game::getFps() {
		return window->getFps();
	}


	void Game::pushLayer(Layer* layer) {
		layersToAdd.push_back(layer);
	}

	void Game::popLayer() {
		if( !layers.empty() ) {
			Layer* layer = layers.back();
			removeLayer(layer);
		}
	}

	void Game::removeLayer(Layer* layer) {
		layersToRemove.push_back(layer);
	}

	void Game::clearLayers() {
		for( auto layer : layers )
			layersToRemove.push_back(layer);
	}

	void Game::exit() {
		window->close();
	}
	
}