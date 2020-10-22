#pragma once

#include <River.h>
#include "RenderSystem.h"


class MainLayer : public River::Layer {

private:
	River::Game* game;
	
	River::TextRenderer* textRenderer = nullptr;
	River::ImageRenderer* imageRenderer = nullptr;
	River::ECS::SpriteAnimationSystem* animationSystem = nullptr;
	RenderSystem* renderSystem = nullptr;
	River::Texture* image_coffee = nullptr;
	River::Texture* image_llama = nullptr;
	River::Texture *image_glyph = nullptr;
	River::Texture *image_blue_circle = nullptr;
	River::Texture *image_purple_circle = nullptr;
	River::Font* font = nullptr;


	River::Texture* santa[8];
	River::SpriteAnimation* santaAnimation;

	River::ECS::Domain domain;
	River::ECS::Entity* santaEntity;


	float x = 0;
	float rotation = 0;

public:

	MainLayer(River::Game* game) : game(game), santa { 
		new River::Texture("assets/santa/Jump (1).png", true),
		new River::Texture("assets/santa/Jump (2).png", true),
		new River::Texture("assets/santa/Jump (3).png", true),
		new River::Texture("assets/santa/Jump (4).png", true),
		new River::Texture("assets/santa/Jump (5).png", true),
		new River::Texture("assets/santa/Jump (6).png", true),
		new River::Texture("assets/santa/Jump (7).png", true),
		new River::Texture("assets/santa/Jump (8).png", true)
	} {}

	void onInitialization() override;
	
	void onUpdate() override;


	void onKeyEvent(River::KeyEvent &e) override;

	void onMouseMoveEvent(River::MouseMoveEvent&) override;

	void onMouseScrollEvent(River::MouseScrollEvent&) override;
	
	void onMouseButtonEvent(River::MouseButtonEvent&) override;


private:
	void createSanta(double x, double y, unsigned int depth);
};



