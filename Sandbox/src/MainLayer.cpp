#include "MainLayer.h"

#include <iostream>

#include "Assets.h"

MainScene::MainScene(River::Layer* layer) 
	: layer(layer)
{
	layer->onUpdate([this]() {this->update();  });
	layer->onCreate([this]() {this->initialize();  });


	// event printing functions
	layer->onKeyEvent([](auto e) {
		std::cout << "KeyEvent: " << (int) e.key << " " << (int) e.action << std::endl;
	});

	layer->onMouseMoveEvent([](auto e) {
		std::cout << "MouseMove: " << (double)e.movementX << " " << (double)e.positionY << std::endl;
	});

	layer->onMouseScrollEvent([](auto e) {
		std::cout << "MouseScroll: " << (double)e.amount << std::endl;
	});

	layer->onMouseButtonEvent([](auto e) {
		std::cout << "MouseButton: " << e.button << " " << (int)e.action << std::endl;
	});
}


void MainScene::createSanta( double x, double y, unsigned int depth) {
	auto entity = domain.createEntity();
	auto transform = entity->addComponent<River::ECS::Transform>();
	transform->x = x;
	transform->y = y;
	transform->width = 150 * GlobalAssets::Textures::CAR->getAspectRatio();
	transform->height = 150 ;
	transform->depth = (double) depth;
	auto sprite = entity->addComponent<River::ECS::Sprite>();
	sprite->texture = GlobalAssets::Textures::CAR;
}


River::ECS::Entity* MainScene::createText(const std::string& msg, unsigned int size, double x, double y) {
	auto entity = domain.createEntity();
	auto transform = entity->addComponent<River::ECS::Transform>();
	transform->x = x;
	transform->y = y;
	transform->depth = 0;
	
	auto text = entity->addComponent<River::ECS::Text>();
	text->font = GlobalAssets::Fonts::ARIAL;
	text->text = msg;
	text->size = size;

	return entity;
}


void MainScene::initialize() {

	std::cout << "Initialized" << std::endl;

	imageRenderer = new River::ImageRenderer(River::Game::getWindow());

	animationSystem = new River::ECS::SpriteAnimationSystem();
	
	//River::FontController::setFontFolder("assets");
	//font = River::FontController::getFont("arial", 50);

	/*image_coffee = new River::Texture("assets/coffee.jpg", false, 100, 100, 760.0/2.0, 506.0/2.0);
	image_llama = new River::Texture("assets/A.png", false);
	image_blue_circle = new River::Texture("assets/BlueAlphaCircle.png", true);
	image_purple_circle = new River::Texture("assets/PurpleAlphaCircle.png", true);*/


	//santaAnimation = new River::SpriteAnimation(0.05f, santa[0], santa[1], santa[2], santa[3], santa[4], santa[5], santa[6], santa[7]);
	

	/*for( int i = 0; i < 10; i++ ) {
		for( int j = 0; j < 10; j++ ) {
			createSanta(-600 + i * 120, -300 + j * 70, 20);
		}
	}*/


	//createSanta(0, 0, 20);
	createText("Hello world", 10, 0, -200);
	createText("Hello world", 20, 0, -100);
	fpsText = createText("Hello world", 30, 0, 0);
	createText("Hello world", 40, 0, 100);
	createText("Hello world", 50, 0, 200);


	camera = new River::Camera(1280, 720);

	domain.clean();

	GlobalAssets::Fonts::ARIAL->load();

}


void MainScene::update() {

	fpsText->getComponent<River::ECS::Text>()->text = std::to_string(River::Game::getFps()) ;

	GlobalAssets::Fonts::ARIAL->load();
	camera->adjustRotation(0.250);

	animationSystem->update(domain, 0.025);

	River::SpriteRenderingSystem::render(camera, domain);
	River::TextRenderingSystem::render(camera, domain);

	domain.clean();

}
