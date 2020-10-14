#include "MainLayer.h"

#include <iostream>



void MainLayer::createSanta( double x, double y, unsigned int depth) {
	auto entity = domain.createEntity();
	entity->addComponent<River::ECS::Sprite>()->texture = santa[0];
	auto transform = entity->addComponent<River::ECS::Transform>();
	transform->x = x;
	transform->y = y;
	transform->depth = (double) depth;
	auto anim = entity->addComponent<River::ECS::AnimatedSprite>();
	anim->animation = santaAnimation;
}


void MainLayer::onInitialization() {

	imageRenderer = new River::ImageRenderer(game->getWindow());
	textRenderer = new River::TextRenderer(game->getWindow());

	animationSystem = new River::ECS::SpriteAnimationSystem();
	renderSystem = new RenderSystem();
	
	River::FontController::setFontFolder("assets");
	font = River::FontController::getFont("arial", 50);

	image_coffee = new River::Texture("assets/coffee.jpg", false, 100, 100, 760.0/2.0, 506.0/2.0);
	image_llama = new River::Texture("assets/A.png", false);
	image_blue_circle = new River::Texture("assets/BlueAlphaCircle.png", true);
	image_purple_circle = new River::Texture("assets/PurpleAlphaCircle.png", true);


	santaSequence = new River::SpriteSequence(santa[0], santa[1], santa[2], santa[3], santa[4], santa[5], santa[6], santa[7]);
	santaAnimation = new River::SpriteAnimation(0.05f, *santaSequence);
	

	for( int i = 0; i < 10; i++ ) {
		for( int j = 0; j < 10; j++ ) {
			createSanta(-600 + i * 120, -300 + j * 70, 20);
		}
	}



	/*santaEntity = domain.createEntity();
	santaEntity->addComponent<River::ECS::Sprite>()->texture = image_purple_circle;
	transform = santaEntity->addComponent<River::ECS::Transform>();
	transform->x = -200;
	transform->y = 100;
	transform->width = 100;
	transform->height = 100;*/


	camera = new River::Camera(1280, 720);
	// camera->setRotation(0.40);
    // camera->setPosition( -100, -100, 0);
	// camera->setZoom(1.5);

	domain.clean();
}


void MainLayer::onUpdate() {
	//camera->adjustRotation(0.010);

	animationSystem->update(domain, 0.025);

	River::SpriteRenderingSystem::render(camera, domain);

	//renderSystem->update(domain, imageRenderer);

	//imageRenderer->setCamera(camera);
	textRenderer->setCamera(camera);

	//imageRenderer->drawRectangle({ 0, 0, 0.1f, 1280, 1, 0, River::Colors::RED });
	//imageRenderer->drawRectangle({ 0, 0, 0.1f, 1, 720, 0, River::Colors::RED });

	textRenderer->drawText(std::to_string(game->getFps()), font, River::Colors::BLACK, 0, 0, 0.0f, River::Align::TOP_CENTER);
	textRenderer->flush();

	//imageRenderer->drawSprite(font->getGlyph("x").sprite, 0, 0, 0.25f, 300, 175, 0);
	//imageRenderer->drawSprite(image_coffee, { 100, 100, 0.0f, 20, 20, 0., {} });
	//imageRenderer->flush();

	domain.clean();
}


void MainLayer::onKeyEvent(River::KeyEvent &e){
	std::cout << "KeyEvent: " << (int) e.key << " " << (int) e.action << std::endl;
}