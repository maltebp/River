#include "MainLayer.h"

#include <iostream>

#include "Assets.h"


MainLayer::MainLayer(const std::string& arg) : 
	music(GlobalAssets::Sounds::TEST_SOUND)
{
	std::cout << "Start arg: " << arg;


	// event printing functions
	
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


void MainLayer::onCreate() {

	std::cout << "Initialized" << std::endl;

	River::AudioSystem::setReferenceDistance(100);

	River::AudioSystem::playAudio(&music);

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


void MainLayer::onUpdate() {

	River::AudioSystem::setListenerPosition(camera->getX(), camera->getY());
	River::AudioSystem::update(0.016666);

	fpsText->getComponent<River::ECS::Text>()->text = std::to_string(River::Game::getFps()) ;

	GlobalAssets::Fonts::ARIAL->load();
	//camera->adjustRotation(0.250);

	River::SpriteAnimationSystem::update(&domain, 0.016);
	River::SpriteRenderingSystem::render(&domain, camera);
	River::TextRenderingSystem::render(&domain, camera);

	domain.clean();

}


void MainLayer::onKeyEvent(River::KeyEvent& e) {
	std::cout << "KeyEvent: " << (int) e.key << " " << (int) e.action << std::endl;
	if (e.key == River::Key::RIGHT)
		camera->adjustX(10);

	if (e.key == River::Key::LEFT)
		camera->adjustX(-10);

	if (e.key == River::Key::UP)
		camera->adjustY(10);

	if (e.key == River::Key::DOWN)
		camera->adjustY(-10);

	if (e.key == River::Key::F && e.action == River::KeyEvent::Action::DOWN) {
		if (music.isPaused()) {
			music.unpause();
		}
		else {
			music.pause();
		}
	}
};

void MainLayer::onMouseMoveEvent(River::MouseMoveEvent& e) {
	std::cout << "MouseMove: " << (double)e.positionX << " " << (double)e.positionY << std::endl;
};

void MainLayer::onMouseScrollEvent(River::MouseScrollEvent& e) {
	std::cout << "MouseScroll: " << (double)e.amount << std::endl;
};

void MainLayer::onMouseButtonEvent(River::MouseButtonEvent& e) {
	std::cout << "MouseButton: " << e.button << " " << (int)e.action << std::endl;
};
