#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cout << "Bad:(";
		return 1;
	}
	else {
		std::cout << "Supper!";
		SDL_Quit();
	}
	return 0;
}
