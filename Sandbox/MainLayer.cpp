#include "MainLayer.h"

void MainLayer::onInitialization() {
	imageRenderer = new River::ImageRenderer(game->getWindow());
	image_coffee = new River::Texture("assets/coffee.jpg");
	image_llama = new River::Texture("assets/llama.jpg");

	camera = new River::Camera();
	//camera->setRotation(2);
	//camera->setPosition(-0.5, -0.5, 0);
	camera->setZoom(2);
}

void MainLayer::onUpdate() {
	imageRenderer->setCamera(camera);
	imageRenderer->drawImage(image_coffee, 0.25,  0.25, 0.5, 0.5, 0.5);
	imageRenderer->drawImage(image_llama, -0.75, -0.75, 0, 0.5, 0.5);
	imageRenderer->flush();
}