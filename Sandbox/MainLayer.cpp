#include "MainLayer.h"

#include <iostream>

#include "River/Graphics/Text/FontLoader.h"

void MainLayer::onInitialization() {
	imageRenderer = new River::ImageRenderer(game->getWindow());
	image_coffee = new River::Sprite("assets/coffee.jpg", 100, 100, 760.0/2.0, 506.0/2.0);
	image_llama = new River::Sprite("assets/A.png");
	//image_glyph = River::FontLoader::loadGlyph("assets/arial.ttf", 'A');


	camera = new River::Camera(1280, 720);
	// camera->setRotation(0.40);
    // camera->setPosition( -100, -100, 0);
	// camera->setZoom(1.5);
}



void MainLayer::onUpdate() {
	//camera->adjustRotation(0.010);

	imageRenderer->setCamera(camera);
	imageRenderer->drawSprite(image_coffee, 0, 0, 0.25f, 300, 175, 0);
	imageRenderer->drawSprite(image_llama, -100, -100, 0.0f, 50, 70, 0);
	imageRenderer->flush();
}


void MainLayer::onKeyEvent(River::KeyEvent &e){
	std::cout << "KeyEvent: " << (int) e.key << " " << (int) e.action << std::endl;
}