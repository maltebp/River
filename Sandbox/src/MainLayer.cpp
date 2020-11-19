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
		std::cout << "MouseMove: " << (double)e.positionX << " " << (double)e.positionY << std::endl;
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
	sprite->texture = GlobalAssets::Textures::SANTA_JUMP_1;

	auto animation = entity->addComponent<River::ECS::SpriteAnimation>();
	animation->animation = GlobalAssets::SpriteAnimation::SANTA_JUMP;

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

	createSanta(0, 0, 20);
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
	//camera->adjustRotation(0.250);

	River::SpriteAnimationSystem::update(domain, 0.016);
	River::SpriteRenderingSystem::render(camera, domain);
	River::TextRenderingSystem::render(camera, domain);

	domain.clean();

}
