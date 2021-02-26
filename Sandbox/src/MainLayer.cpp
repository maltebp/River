#include "MainLayer.h"

#include <iostream>

#include "Assets.h"

River::AudioPlayer countdown;


MainLayer::MainLayer(const std::string& arg)
{
	River::Window::resolutionChangedListeners.add(this, [](River::ResolutionEvent& event) {
		auto resolution = event.getResolution();
		printf("Resolution listener: %ix%i\n", resolution.width, resolution.height);
		});
	River::Window::viewportChangedListeners.add(this, [](River::ResolutionEvent& event) {
		auto resolution = event.getResolution();
		printf("Viewport listener: %ix%i\n", resolution.width, resolution.height);
	});
	std::cout << "Start arg: " << arg;
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
	text->color = River::Colors::BLACK;

	return entity;
}


void MainLayer::onCreate() {

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

	// Background
	{ 
		auto entity = domain.createEntity();
		auto transform = entity->addComponent<River::ECS::Transform>();
		transform->width = 10000;
		transform->height = 10000;
		transform->depth = 100;
		auto sprite = entity->addComponent<River::ECS::Sprite>();
		sprite->texture = GlobalAssets::Textures::PIXEL;
	}
	
	camera = new River::Camera(1280, 720);

	domain.clean();

	GlobalAssets::Fonts::ARIAL->load();

}


void MainLayer::onUpdate() {

	River::AudioListener::setPosition(camera->getX(), camera->getY());
	//River::AudioListener::setDepth(400);

	fpsText->getComponent<River::ECS::Text>()->text = std::to_string(River::Window::getFps()) ;

	GlobalAssets::Fonts::ARIAL->load();

	River::SpriteAnimationSystem::update(&domain, 0.016);
	River::SpriteRenderingSystem::render(&domain, camera);
	River::TextRenderingSystem::render(&domain, camera);

	domain.clean();
}

static double audioDepth = 0;

bool listenerAdded = false;

void MainLayer::onKeyEvent(River::KeyEvent& e) {
	//std::cout << "KeyEvent: " << (int) e.key << " " << (int) e.action << std::endl;

	if (e.key == River::Key::RIGHT)
		camera->adjustX(10);

	if (e.key == River::Key::LEFT)
		camera->adjustX(-10);

	if (e.key == River::Key::UP)
		camera->adjustY(10);

	if (e.key == River::Key::DOWN)
		camera->adjustY(-10);

	if( e.key == River::Key::ESCAPE && e.action == River::KeyEvent::Action::DOWN ) {
		River::Game::exit();
		return;
	}

	if( e.key == River::Key::B && e.action == River::KeyEvent::Action::DOWN ) {
		if( River::Window::isFullscreen() ) {
			printf("Disabling fullscreen\n");
			River::Window::disableFullscreen({ 400, 400 });
			auto actualResolution = River::Window::getResolution();
			printf("Windowed resolution: %ix%i\n", actualResolution.width, actualResolution.height);
		}
		else {
			printf("Setting fullscreen\n");
			River::Window::enableFullscreen({800, 600});
			auto actualResolution = River::Window::getResolution();
			printf("Fullscreen resolution: %ix%i\n", actualResolution.width, actualResolution.height);
		}
	}	


	if( e.key == River::Key::X && e.action == River::KeyEvent::Action::DOWN ) {
		River::Window::setResolution({1920, 1080});
		auto actualResolution = River::Window::getResolution();
		std::cout << "Resolution was set to: " << actualResolution.width << "x" << actualResolution.height << std::endl;
	}

	if( e.key == River::Key::L && e.action == River::KeyEvent::Action::DOWN ) {
		auto asset = GlobalAssets::Sounds::COINS;
		if( asset->isLoaded() ) {
			asset->unload();
			std::cout << "Asset unloaded" << std::endl;
		}
		else {
			asset->load();
			std::cout << "Asset loaded" << std::endl;
		}
	}


	if( e.key == River::Key::I && e.action == River::KeyEvent::Action::DOWN ) {
		if( countdown.isPaused() ) {
			countdown.unpause();
			std::cout << "Unpaused countdown" << std::endl;
		}
		else {
			countdown.pause();
			if( countdown.isPaused() )
				std::cout << "Paused countdown" << std::endl;
			else
				std::cout << "Attempted to pause countdown" << std::endl;
		}
	}

	if( e.key == River::Key::O && e.action == River::KeyEvent::Action::DOWN ) {
		countdown.stop();
		std::cout << "Stopped countdown" << std::endl;
	}


	if( e.key == River::Key::P && e.action == River::KeyEvent::Action::DOWN ) {
		countdown.play(GlobalAssets::Sounds::COUNTDOWN);
		std::cout << "Started countdown" << std::endl;
	}

	if( e.key == River::Key::U && e.action == River::KeyEvent::Action::DOWN ) {
		countdown.setTime(7);
		std::cout << "Set countdown to 7 seconds" << std::endl;
	}


	if (e.key == River::Key::D && e.action == River::KeyEvent::Action::DOWN) {
		River::AudioPlayer* audio = new River::AudioPlayer();
		audio->setSpatial(true);
		audio->setRange(500);
		audio->setSize(499);
		audio->setDepth(audioDepth);
		audio->onFinish([](auto i) {
			delete i;
			std::cout << "Audio has finished!" << std::endl;
		});
		audio->play(GlobalAssets::Sounds::COINS);
	}

	if (e.key == River::Key::F && e.action == River::KeyEvent::Action::DOWN) {
		River::AudioPlayer* audio = new River::AudioPlayer();
		//audio->loop(true);
		audio->play(GlobalAssets::Sounds::COUNTDOWN);
	}

	if (e.key == River::Key::G && e.action == River::KeyEvent::Action::DOWN) {
		River::AudioPlayer* audio = new River::AudioPlayer();
		//audio->loop(true);
		audio->setVolume(0.5);
		audio->play(GlobalAssets::Sounds::CLASSICAL_MUSIC);
	}

	if( e.key == River::Key::W && e.action == River::KeyEvent::Action::PRESSED ){
		audioDepth += 5;
		std::cout << "Audio depth: " << audioDepth << std::endl;
	}

	if( e.key == River::Key::E && e.action == River::KeyEvent::Action::PRESSED ){
		audioDepth -= 5;
		std::cout << "Audio depth: " << audioDepth << std::endl;
	}

	if( e.key == River::Key::A && e.action == River::KeyEvent::Action::PRESSED ){
		camera->adjustRotation(5);
		River::AudioListener::setRotation(camera->getRotation());
		std::cout << "Rotation: " << camera->getRotation() << std::endl;
	}

	if( e.key == River::Key::S && e.action == River::KeyEvent::Action::PRESSED ){
		camera->adjustRotation(-5);
		River::AudioListener::setRotation(camera->getRotation());
		std::cout << "Rotation: " << camera->getRotation() << std::endl;
	}
	
};


void MainLayer::onMouseMoveEvent(River::MouseMoveEvent& e) {
	//std::cout << "MouseMove: " << (double)e.positionX << " " << (double)e.positionY << std::endl;
};

void MainLayer::onMouseScrollEvent(River::MouseScrollEvent& e) {
	//std::cout << "MouseScroll: " << (double)e.amount << std::endl;
};

void MainLayer::onMouseButtonEvent(River::MouseButtonEvent& e) {
	//std::cout << "MouseButton: " << e.button << " " << (int)e.action << std::endl;
};
