

#include <River.h>

#include <iostream>


#include "Assets.h"
#include "MainLayer.h"


using River::Game;
using River::Window;


void main() {

	Window::setTitle("Sandbox Game");
	Window::setResolution({ 1280, 720 });

	Game::start([]() {
		GlobalAssets::COLLECTION->load();
		River::SpriteRenderingSystem::initialize();
		River::TextRenderingSystem::initialize();
		Game::pushLayer<MainLayer>("This is a layer starting argument!");
	});


	std::cout << "Stopping Sandbox" << std::endl;
}


