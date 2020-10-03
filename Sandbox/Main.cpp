

#include <River.h>

#include <iostream>

#include "MainLayer.h"



class SandboxGame : public River::Game {

private:
	River::ECS::Domain domain;
	

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