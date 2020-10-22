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

			rootLayer->clean();

			// Fire Key Events 
			auto keyEvents = window->getKeyEvents();
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


			rootLayer->update();
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


	Layer* Game::pushLayer() {
		return rootLayer->pushLayer();
	}


	void Game::removeLayer(Layer* layer) {
		rootLayer->removeLayer(layer);
	}

	void Game::clearLayers() {
		rootLayer->clearLayers();
	}

	void Game::exit() {
		window->close();
	}
	
}