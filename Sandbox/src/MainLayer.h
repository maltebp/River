#pragma once

#include <River.h>



class MainScene {
public:

	MainScene(River::Layer* layer);

	void update();
	void initialize();

private:
	void createSanta(double x, double y, unsigned int depth);
	void createText(const std::string& text, unsigned int size, double x, double y);


private:
	River::ImageRenderer* imageRenderer = nullptr;
	River::ECS::SpriteAnimationSystem* animationSystem = nullptr;
	River::Texture* image_coffee = nullptr;
	River::Texture* image_llama = nullptr;
	River::Texture* image_glyph = nullptr;
	River::Texture* image_blue_circle = nullptr;
	River::Texture* image_purple_circle = nullptr;
	//River::Font* font = nullptr;




	River::Texture* santa[8];
	River::SpriteAnimation* santaAnimation;

	River::ECS::Domain domain;
	River::ECS::Entity* santaEntity;

	River::Camera* camera;

	float x = 0;
	float rotation = 0;

	River::Layer* layer;

};


