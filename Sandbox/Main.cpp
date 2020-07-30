
#include <River.h>

#include <stdio.h>

class SandboxGame : public River::Game {

public:
	void update() override {
		printf("Doing some update!");
	}
};


int main() {
	
	printf("Starting Sandbox!");

	SandboxGame game;

	game.start();

}