#include "MainLayer.h"

#include <iostream>


void MainLayer::onInitialization() {

	imageRenderer = new River::ImageRenderer(game->getWindow());
	textRenderer = new River::TextRenderer(game->getWindow());
	
	River::FontController::setFontFolder("assets");
	font = River::FontController::getFont("arial", 50);

	image_coffee = new River::Sprite("assets/coffee.jpg", 100, 100, 760.0/2.0, 506.0/2.0);
	image_llama = new River::Sprite("assets/A.png");

	camera = new River::Camera(1280, 720);
	// camera->setRotation(0.40);
    // camera->setPosition( -100, -100, 0);
	// camera->setZoom(1.5);
}


void MainLayer::onUpdate() {
	//camera->adjustRotation(0.010);

	imageRenderer->setCamera(camera);
	textRenderer->setCamera(camera);

	imageRenderer->drawRectangle({ 0, 0, 0.1f, 1280, 1, 0, River::Colors::RED });
	imageRenderer->drawRectangle({ 0, 0, 0.1f, 1, 720, 0, River::Colors::RED });


	textRenderer->drawText(std::to_string(game->getFps()), font, River::Colors::WHITE, 0, 0, 0.0f, River::Align::TOP_CENTER);

	//imageRenderer->drawSprite(font->getGlyph("x").sprite, 0, 0, 0.25f, 300, 175, 0);
	//imageRenderer->drawSprite(image_coffee, { 100, 100, 0.0f, 20, 20, 0., {} });
	imageRenderer->flush();
	textRenderer->flush();
}


void MainLayer::onKeyEvent(River::KeyEvent &e){
	std::cout << "KeyEvent: " << (int) e.key << " " << (int) e.action << std::endl;
}