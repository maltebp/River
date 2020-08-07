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
	// TODO: Fill in destructor stuff
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
		
		onUpdate();
	}

	printf("Game loop stopped\n");
}


