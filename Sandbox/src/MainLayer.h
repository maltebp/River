#pragma once

#include <River.h>
#include <River/Graphics/GLTextureRenderer.h>
#include <string>

class MainLayer : public River::Layer {
public:

	MainLayer(River::Viewport* mainViewport);

	void onCreate() override;

	void onUpdate() override;

	void onUpdateImGui() override;

	void onKeyDownEvent(River::KeyEvent& e);

private:

	void createSanta(double x, double y, unsigned int depth);

	River::ECS::Entity* createText(const std::string& text, unsigned int size, double x, double y);

private:

	River::Viewport* mainViewport;

	River::ImageRenderer* imageRenderer = nullptr;

	River::GLTextureRenderer textureRenderer;

	River::ECS::Entity* fpsText = nullptr;

	River::ECS::Domain domain;

	River::ECS::Entity* santaEntity;

	River::Camera* camera;

	float x = 0;
	float rotation = 0;

	River::Layer* layer;

};


