#include "MainLayer.h"

void MainLayer::onInitialization() {
	imageRenderer = new River::ImageRenderer(game->getWindow());
	image_coffee = new River::Texture("assets/coffee.jpg");
	image_llama = new River::Texture("assets/llama.jpg");

	camera = new River::Camera(1280, 720);
	// camera->setRotation(0.40);
    // camera->setPosition( -100, -100, 0);
	// camera->setZoom(1.5);
}



void MainLayer::onUpdate() {
	camera->adjustRotation(0.010);

	imageRenderer->setCamera(camera);
	imageRenderer->drawImage(image_coffee, 0, 0, 0.0f, 300, 175, 480);
	imageRenderer->drawImage(image_llama, -50, -25, 0.25f, 300, 175, 45);
	imageRenderer->flush();
}


void MainLayer::onKeyEvent(River::KeyEvent &e){
	std::cout << "KeyEvent: " << (int) e.key << " " << (int) e.action << std::endl;
}