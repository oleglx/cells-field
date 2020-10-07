#include <iostream>
#include <utility>
#include <vector>
#include "Cell.h"
#include "CellController.h"
#include "GraphicsController.h"
#include "SDL.h"

// Constants of the field

const int GRID_SIZE_W = 128;
const int GRID_SIZE_H = 96;
const int GRID_CELL_S = 8;

// Main Function

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);
	GraphicsController currentGraphicsController(GRID_SIZE_W, GRID_SIZE_H, GRID_CELL_S);
	currentGraphicsController.InitializeController();
	currentGraphicsController.InitializeBackground();

	for (int i = 0;; i++) {
		currentGraphicsController.DrawNextFrame();
		SDL_Delay(1);
	}


	return 0;
}