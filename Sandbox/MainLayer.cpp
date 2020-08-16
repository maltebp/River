#include "MainLayer.h"

void MainLayer::onInitialization() {
	imageRenderer = new River::ImageRenderer(game->getWindow());
	image_coffee = new River::Texture("assets/coffee.jpg");
	image_llama = new River::Texture("assets/llama.jpg");
}

void MainLayer::onUpdate() {
	imageRenderer->drawImage(image_coffee, 0, 0, 0.5, 0.5, 0.5);
	imageRenderer->drawImage(image_llama, 0.25, 0.25, 0, 0.5, 0.5);
	imageRenderer->flush();
}