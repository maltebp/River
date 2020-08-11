#include "MainLayer.h"

void MainLayer::onInitialization() {
	imageRenderer = new River::ImageRenderer(game->getWindow());
	image_coffee = new River::Texture("assets/coffee.jpg");
	image_llama = new River::Texture("assets/llama.jpg");
}

void MainLayer::onUpdate() {
	x += 0.005;
	if (x > 1.0) x = -1.50;
	imageRenderer->drawImage(image_coffee, x-0.5, -0.25, 0.5, 0.5);
	imageRenderer->drawImage(image_llama, -(x-0.5), 0.25, 0.5, 0.5);
	imageRenderer->flush();
}