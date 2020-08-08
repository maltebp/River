
#include <River.h>
#include <iostream>

#include "MainLayer.h"


class SandboxGame : public River::Game {

private:

	

public:

	SandboxGame() : Game("Sandbox Game") {
	
	}

	void onInitialization() override {
		pushLayer(new MainLayer(this));
	}
};


int main() {
	printf("Starting Sandbox!\n");

	SandboxGame game;
	game.start();	
}