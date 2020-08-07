
#include <River.h>
#include <iostream>


class SandboxGame : public River::Game {

private:

	River::ImageRenderer* imageRenderer;
	River::Texture* image_coffee;

public:

	SandboxGame() : Game("Sandbox Game") {
		
	}

	void onInitialization() override {
		imageRenderer = new River::ImageRenderer(getWindow());
		image_coffee = new River::Texture("assets/coffee.jpg");
	}

	void onUpdate() override {
		
		imageRenderer->drawImage(image_coffee, 0, 0, 0.5, 0.5);

		// TODO: Do stuff here!
	}
};


int main() {
	printf("Starting Sandbox!\n");

	SandboxGame game;
	game.start();

	try {
		
	}
	catch (River::Exception e) {
		std::cerr <<  e.what() << std::endl;;
	}	
	
}