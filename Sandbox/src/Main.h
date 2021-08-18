

#include <River.h>

#include <iostream>


#include "Assets.h"
#include "EditorLayer.h"
#include "MainLayer.h"


using River::Game;
using River::Window;

#define EDITOR_MODE


int main() {

	Window::setTitle("Sandbox Game");
	Window::setResolution({ 1280, 720 });
	
	#ifdef EDITOR_MODE
		Window::disableMainViewport();
	#endif

	Game::enableImGui();

	Game::start([]() {
		GlobalAssets::COLLECTION->load();
		River::SpriteRenderingSystem::initialize();
		River::TextRenderingSystem::initialize();	

		#ifdef EDITOR_MODE
			Game::pushLayer<EditorLayer>();
		#else
			Game::pushLayer<MainLayer>(Window::getMainViewport());
		#endif
	});

	std::cout << "Stopping Sandbox" << std::endl;
}


