#pragma once

#include <River.h>

class MainLayer : public River::Layer {

private:
	River::Game* game;
	
	River::ImageRenderer* imageRenderer = nullptr;
	River::Texture* image_coffee = nullptr;

public:

	MainLayer(River::Game* game) : game(game) {}

	void onInitialization() override;
	
	void onUpdate() override;

};

