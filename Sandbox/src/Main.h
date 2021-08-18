

#include <River.h>

#include <iostream>


#include "Assets.h"
#include "MainLayer.h"


using River::Game;
using River::Window;


int main() {

	Window::setTitle("Sandbox Game");
	Window::setResolution({ 1280, 720 });

	// Window::disableMainViewport();
	Game::enableImGui();

	Game::start([]() {
		GlobalAssets::COLLECTION->load();
		River::SpriteRenderingSystem::initialize();
		River::TextRenderingSystem::initialize();	
		Game::pushLayer<MainLayer>(Window::getMainViewport());
	});

	std::cout << "Stopping Sandbox" << std::endl;
}


