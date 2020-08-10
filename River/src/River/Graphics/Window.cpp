#include "Window.h"

#include <string>


// Handles the error callback
static int glfwErrorCode = 0;
static std::string glfwErrorMsg;
static void glfwErrorCallback(int errCode, const char* errStr) {
	glfwErrorCode = errCode;
	glfwErrorMsg = std::string(errStr);
}


River::Window::Window(std::string title) {
	this->title = title;

	// TODO: Make the user able to adjust this
	width = 1280;
	height = 720;

	// Initialize glfw
	if (!glfwInit()) {
		throw River::Exception("GLFW initialization error '" + glfwErrorMsg + "'" + " (code: " + std::to_string(glfwErrorCode) + ")");
	}


	glfwSetErrorCallback(glfwErrorCallback);
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindow = glfwCreateWindow(width, height, this->title.c_str(), NULL, NULL);
	if (!glfwWindow) {
		glfwTerminate();
		throw River::Exception("GLFW initialization error '" + glfwErrorMsg + "'" + " (code: " + std::to_string(glfwErrorCode) + ")");
	}

	// Set window to current
	glfwMakeContextCurrent(glfwWindow);

	// Get number of texture slots
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &numTextureSlots);
}


River::Window::~Window() {

}



void River::Window::clear() {
	glfwSwapBuffers(this->glfwWindow);
	glfwPollEvents();
	GL(glClear(GL_COLOR_BUFFER_BIT));
}


bool River::Window::shouldClose() {
	return glfwWindowShouldClose(glfwWindow);
}


void River::Window::clearDepth() {
	// TODO: Implement depth buffer stuff here
}
