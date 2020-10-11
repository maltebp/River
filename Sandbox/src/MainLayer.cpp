#include "MainLayer.h"

#include <iostream>
#include "Components/Transform.h"



void MainLayer::onInitialization() {

	imageRenderer = new River::ImageRenderer(game->getWindow());
	textRenderer = new River::TextRenderer(game->getWindow());

	animationSystem = new River::ECS::SpriteAnimationSystem();
	renderSystem = new RenderSystem();
	
	River::FontController::setFontFolder("assets");
	font = River::FontController::getFont("arial", 50);

	image_coffee = new River::Texture("assets/coffee.jpg", 100, 100, 760.0/2.0, 506.0/2.0);
	image_llama = new River::Texture("assets/A.png");


	santaSequence = new River::SpriteSequence(santa[0], santa[1], santa[2], santa[3], santa[4], santa[5], santa[6], santa[7]);
	santaAnimation = new River::SpriteAnimation(0.05f, *santaSequence);
	
	santaEntity = domain.createEntity();
	santaEntity->addComponent<River::ECS::Sprite>();
	santaEntity->addComponent<Transform>()->x = 400;
	auto anim = santaEntity->addComponent<River::ECS::AnimatedSprite>();
	anim->animation = santaAnimation;

	santaEntity = domain.createEntity();
	santaEntity->addComponent<River::ECS::Sprite>();
	santaEntity->addComponent<Transform>()->x = 200;
	anim = santaEntity->addComponent<River::ECS::AnimatedSprite>();
	anim->animation = santaAnimation;

	santaEntity = domain.createEntity();
	santaEntity->addComponent<River::ECS::Sprite>();
	santaEntity->addComponent<Transform>()->x = -400;
	anim = santaEntity->addComponent<River::ECS::AnimatedSprite>();
	anim->animation = santaAnimation;


	camera = new River::Camera(1280, 720);
	// camera->setRotation(0.40);
    // camera->setPosition( -100, -100, 0);
	// camera->setZoom(1.5);

	domain.clean();
}


void MainLayer::onUpdate() {
	//camera->adjustRotation(0.010);

	animationSystem->update(domain, 0.017);
	renderSystem->update(domain, imageRenderer);

	imageRenderer->setCamera(camera);
	textRenderer->setCamera(camera);

	imageRenderer->drawRectangle({ 0, 0, 0.1f, 1280, 1, 0, River::Colors::RED });
	imageRenderer->drawRectangle({ 0, 0, 0.1f, 1, 720, 0, River::Colors::RED });

	textRenderer->drawText(std::to_string(game->getFps()), font, River::Colors::WHITE, 0, 0, 0.0f, River::Align::TOP_CENTER);

	//imageRenderer->drawSprite(font->getGlyph("x").sprite, 0, 0, 0.25f, 300, 175, 0);
	//imageRenderer->drawSprite(image_coffee, { 100, 100, 0.0f, 20, 20, 0., {} });
	imageRenderer->flush();
	textRenderer->flush();

	domain.clean();
}


void MainLayer::onKeyEvent(River::KeyEvent &e){
	std::cout << "KeyEvent: " << (int) e.key << " " << (int) e.action << std::endl;
}