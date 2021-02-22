#pragma once

#include <River.h>

#include <string>

class MainLayer : public River::Layer {
public:

	MainLayer(const std::string&);

	void onCreate() override;
	void onUpdate() override;

	void onKeyEvent(River::KeyEvent& e) override;
	void onMouseMoveEvent(River::MouseMoveEvent& e) override;
	void onMouseScrollEvent(River::MouseScrollEvent& e) override;
	void onMouseButtonEvent(River::MouseButtonEvent& e) override;


private:
	void createSanta(double x, double y, unsigned int depth);
	River::ECS::Entity* createText(const std::string& text, unsigned int size, double x, double y);


private:
	River::ImageRenderer* imageRenderer = nullptr;

	River::ECS::Entity* fpsText = nullptr;

	River::ECS::Domain domain;
	River::ECS::Entity* santaEntity;

	River::Camera* camera;

	float x = 0;
	float rotation = 0;

	River::Layer* layer;

	River::Window::ResolutionListener resolutionCallback;
};


