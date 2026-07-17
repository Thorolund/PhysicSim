#include <SDL.h>
#include <iostream>
#include "logger.h"

int main(int argc, char* argv[]) {
	if (!SDL_Init(SDL_INIT_EVERYTHING)) {
		log("main", "sdl load error", "load sdl", "check lib");
		return 1;
	}
	else {
		std::cout << "Supper!";
		SDL_Quit();
	}
	return 0;
}
