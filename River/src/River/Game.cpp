#pragma once

#include "Game.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include "Event/MouseEvent/MouseEventController.h"


namespace River {
	
	// The current game
	Game* Game::game = nullptr;

	Game* Game::getGame() {
		return game;
	}

	River::Game::Game(std::string title) {
		this->title = title;

		if( game != nullptr )
			throw new River::Exception("A game has already been created!");
		game = this;

		this->window = new Window(this->title, 1280, 720);

		// Initialize glew
		const GLenum glewResult = glewInit();
		if( glewResult != GLEW_OK ) {
			std::stringstream msgStream;
			msgStream << "GLEW initialization error '" << glewGetErrorString(glewResult) << "'";
			throw River::Exception(msgStream.str());
		}

		std::cout << "GLEW initialized" << std::endl;
		std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "Graphics card: " << glGetString(GL_RENDERER)<<std::endl;
	}


	River::Game::~Game() {
		clearLayers();
		clearOverlays();
	}

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


	River::Window* River::Game::getWindow() {
		return window;
	}


	double Game::getFps() {
		return window->getFps();
	}





	void River::Game::start() {

		printf("Starting River game loop\n");

		onInitialization();

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
				for( auto it = overlays.rbegin(); it != overlays.rend(); it++ ) {
					if( keyEvent.isConsumed() ) break;
					(*it)->onKeyEvent(keyEvent);
				}
				for( auto it = layers.rbegin(); it != layers.rend(); it++ ) {
					if( keyEvent.isConsumed() ) break;
					(*it)->onKeyEvent(keyEvent);
				}
			}

			// Fire Mouse Movement Events
			if( MouseEventController::mouseMovementOccured() ) {
				auto mouseMoveEvent = MouseEventController::getMouseMoveEvent();
				
				for( auto it = overlays.rbegin(); it != overlays.rend(); it++ ) {
					if( mouseMoveEvent.isConsumed() ) break;
					(*it)->onMouseMoveEvent(mouseMoveEvent);
				}
				for( auto it = layers.rbegin(); it != layers.rend(); it++ ) {
					if( mouseMoveEvent.isConsumed() ) break;
					(*it)->onMouseMoveEvent(mouseMoveEvent);
				}
			}

			for( auto& layer : layers ) {
				layer->update();
				window->clearDepth();
			}

			for( auto& overlay : overlays ) {
				overlay->update();
				window->clearDepth();
			}




		}

		printf("Game loop stopped\n");
	}



	void River::Game::pushLayer(Layer* layer) {
		layersToAdd.push_back(layer);
	}

	void River::Game::popLayer() {
		if( !layers.empty() ) {
			Layer* layer = layers.back();
			removeLayer(layer);
		}
	}

	void River::Game::removeLayer(Layer* layer) {
		layersToRemove.push_back(layer);
	}

	void River::Game::clearLayers() {
		for( auto layer : layers )
			layersToRemove.push_back(layer);
	}



	void River::Game::pushOverlay(Layer* overlay) {
		overlays.push_back(overlay);
		overlay->initialize();
	}

	void River::Game::popOverlay() {
		if( !overlays.empty() ) {
			Layer* overlay = overlays.back();
			overlays.pop_back();
			overlay->terminate();
			delete overlay;
		}
	}

	void River::Game::clearOverlays() {
		for( auto& overlay : overlays ) {
			overlay->terminate();
			delete overlay;
		}
		overlays.clear();
	}



	void Game::exit() {
		window->close();
	}



	// TODO: testing new start method

	
}


extern void startGame();

int main(int argc, char** argv) {
	startGame();

	return 1;
}