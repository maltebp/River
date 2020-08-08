#include "MainLayer.h"

void MainLayer::onInitialization() {
	imageRenderer = new River::ImageRenderer(game->getWindow());
	image_coffee = new River::Texture("assets/coffee.jpg");
}

void MainLayer::onUpdate() {
	imageRenderer->drawImage(image_coffee, 0, 0, 0.5, 0.5);
}