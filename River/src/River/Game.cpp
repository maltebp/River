#pragma once


#include "Game.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>


River::Game::Game(std::string title) {
	this->title = title;
}


River::Game::~Game() {
	clearLayers();
	clearOverlays();
}

static int glfwErrorCode = 0;
static std::string glfwErrorMsg;

static void glfwErrorCallback(int errCode, const char* errStr) {
	glfwErrorCode = errCode;
	glfwErrorMsg = std::string(errStr);
}

static void printGLErrors() {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "GL ERROR: " << std::hex << err << std::endl;
	}
}


River::Window* River::Game::getWindow() {
	return window;
}



void River::Game::start() {

	printf("Starting River game loop\n");
	
	this->window = new Window(this->title);

	// Initialize glew
	const GLenum glewResult = glewInit();
	if (glewResult != GLEW_OK) {
		std::stringstream msgStream;
		msgStream << "GLEW initialization error '" << glewGetErrorString(glewResult) << "'";
		throw River::Exception(msgStream.str());
	}

	onInitialization();

	printf("Starting game loop\n");
	while( !window->shouldClose() ) {
		window->clear();

		for( auto &layer : layers ) {
			layer->update();
		}
		for (auto& overlay : overlays) {
			overlay->update();
		}
	}

	printf("Game loop stopped\n");
}



void River::Game::pushLayer(Layer* layer){
	layers.push_back(layer);
	layer->initialize();
}

void River::Game::popLayer(){
	if (!layers.empty()) {
		Layer* layer = layers.back();
		layers.pop_back();
		layer->terminate();
		delete layer;
	}
}

void River::Game::clearLayers(){
	for (auto& layer : layers) {
		layer->terminate();
		delete layer;
	}
	layers.clear();
}

void River::Game::pushOverlay(Layer* overlay){
	overlays.push_back(overlay);
	overlay->initialize();
}

void River::Game::popOverlay() {
	if (!overlays.empty()) {
		Layer* overlay = overlays.back();
		overlays.pop_back();
		overlay->terminate();
		delete overlay;
	}
}

void River::Game::clearOverlays(){
	for (auto& overlay : overlays) {
		overlay->terminate();
		delete overlay;
	}
	overlays.clear();
}

