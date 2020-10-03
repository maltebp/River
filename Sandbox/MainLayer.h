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
	River::SubTexture* image_coffee = nullptr;
	River::SubTexture* image_llama = nullptr;
	River::SubTexture *image_glyph = nullptr;
	River::Font* font = nullptr;


	River::SubTexture* santa[8];
	River::SpriteSequence* santaSequence;
	River::SpriteAnimation* santaAnimation;

	River::ECS::Domain domain;
	River::ECS::Entity* santaEntity;


	float x = 0;
	float rotation = 0;

public:

	MainLayer(River::Game* game) : game(game), santa { 
		new River::SubTexture("assets/santa/Jump (1).png"),
		new River::SubTexture("assets/santa/Jump (2).png"),
		new River::SubTexture("assets/santa/Jump (3).png"),
		new River::SubTexture("assets/santa/Jump (4).png"),
		new River::SubTexture("assets/santa/Jump (5).png"),
		new River::SubTexture("assets/santa/Jump (6).png"),
		new River::SubTexture("assets/santa/Jump (7).png"),
		new River::SubTexture("assets/santa/Jump (8).png")
	} {}

	void onInitialization() override;
	
	void onUpdate() override;

	void onKeyEvent(River::KeyEvent &e) override;

};

