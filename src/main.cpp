#include "Game.h"

int main() {
	vuk::Platform* platform = vuk::Platform::instance();
	platform->createAndShowWindow("Resta Giù! v1.0", 1920, 1080, 800, 450);
	platform->initFrameTimer(16);
	
	Game game;
	game.init();
	
	GameInput inputLogic(&game);
	GameFrame frameLogic(&game);
	
	while(!inputLogic.shouldClose()) {
		platform->pollEvents(&inputLogic);
		platform->executeFrame(&frameLogic);
	}
	
	platform->quit();
	
	return 0;
}