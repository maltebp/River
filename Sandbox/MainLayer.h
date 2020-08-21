#pragma once

#include <River.h>

class MainLayer : public River::Layer {

private:
	River::Game* game;
	
	River::TextRenderer* textRenderer = nullptr;
	River::ImageRenderer* imageRenderer = nullptr;
	River::Sprite* image_coffee = nullptr;
	River::Sprite* image_llama = nullptr;
	River::Sprite *image_glyph = nullptr;
	River::Font* font = nullptr;

	float x = 0;
	float rotation = 0;

public:

	MainLayer(River::Game* game) : game(game) {}

	void onInitialization() override;
	
	void onUpdate() override;

	void onKeyEvent(River::KeyEvent &e) override;

};

