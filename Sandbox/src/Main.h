

#include <River.h>

#include <iostream>

#include "MainLayer.h"

using River::Game;


void main() {

	Game::setTitle("Sandbox Game");
	Game::setWindowSize(1280, 720);

	Game::start([]() {
		River::SpriteRenderingSystem::initialize(Game::getWindow());
		Game::pushLayer(new MainLayer());
	});


	std::cout << "Stopping Sandbox" << std::endl;
}


