#include "MainLayer.h"

#include <iostream>

#include "Assets.h"

MainLayer::MainLayer(const std::string& arg) {
	std::cout << "Start arg: " << arg;

	onUpdate = [this](){ this->update(); };
	onCreate = [this](){ this->initialize(); };


	// event printing functions
	onKeyEvent = [](auto e) {
		std::cout << "KeyEvent: " << (int) e.key << " " << (int) e.action << std::endl;
	};

	onMouseMoveEvent = [](auto e) {
		std::cout << "MouseMove: " << (double)e.positionX << " " << (double)e.positionY << std::endl;
	};

	onMouseScrollEvent = [](auto e) {
		std::cout << "MouseScroll: " << (double)e.amount << std::endl;
	};

	onMouseButtonEvent = [](auto e) {
		std::cout << "MouseButton: " << e.button << " " << (int)e.action << std::endl;
	};
}


void MainLayer::createSanta( double x, double y, unsigned int depth) {
	auto entity = domain.createEntity();
	auto transform = entity->addComponent<River::ECS::Transform>();
	transform->x = x;
	transform->y = y;
	transform->width = 150 * GlobalAssets::Textures::CAR->getAspectRatio();
	transform->height = 150 ;
	transform->depth = (double) depth;

	auto sprite = entity->addComponent<River::ECS::Sprite>();
	sprite->texture = GlobalAssets::Textures::SANTA_JUMP_1;
	sprite->opacity = 0.5;

	auto animation = entity->addComponent<River::ECS::SpriteAnimation>();
	animation->animation = GlobalAssets::SpriteAnimation::SANTA_JUMP;

}


River::ECS::Entity* MainLayer::createText(const std::string& msg, unsigned int size, double x, double y) {
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


void MainLayer::initialize() {

	std::cout << "Initialized" << std::endl;

	createSanta(0, 0, 20);
	createSanta(50, 0, 20);
	createSanta(650, 0, 20);
	createSanta(650, 320, 20);
	createSanta(650, 0, 20);
	createSanta(650, -320, 20);
	createText("Hello world", 10, 0, -200);
	createText("Hello world", 20, 0, -100);
	fpsText = createText("Hello world", 30, 0, 0);
	createText("Hello world", 40, 0, 100);
	createText("Hello world", 50, 0, 200);


	camera = new River::Camera(1280, 720);

	domain.clean();

	GlobalAssets::Fonts::ARIAL->load();

}


void MainLayer::update() {

	fpsText->getComponent<River::ECS::Text>()->text = std::to_string(River::Game::getFps()) ;

	GlobalAssets::Fonts::ARIAL->load();
	//camera->adjustRotation(0.250);

	River::SpriteAnimationSystem::update(&domain, 0.016);
	River::SpriteRenderingSystem::render(&domain, camera);
	River::TextRenderingSystem::render(&domain, camera);

	domain.clean();

}
