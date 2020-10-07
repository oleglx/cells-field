#include "SDL.h"
#include "GraphicsController.h"
#include "CellController.h"

GraphicsController::GraphicsController(int GRID_SIZE_W, int GRID_SIZE_H, int GRID_CELL_S) {
	this->GRID_SIZE_W = GRID_SIZE_W;
	this->GRID_SIZE_H = GRID_SIZE_H;
	this->GRID_CELL_S = GRID_CELL_S;
	this->SCREEN_WIDTH = GRID_CELL_S * GRID_SIZE_W;
	this->SCREEN_HEIGHT = GRID_CELL_S * GRID_SIZE_H;
}

void GraphicsController::InitializeController() {
	window = SDL_CreateWindow("Generate", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	currentCellController = new CellController(GRID_SIZE_W, GRID_SIZE_H, GRID_CELL_S);
	currentCellController->GenerateCells(100, false);
}

void GraphicsController::InitializeBackground() {
	CreateSurface(true);
	DrawGrid();
	backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
}

void GraphicsController::DrawNextFrame() {
	DrawCells();
	pawnTexture = SDL_CreateTextureFromSurface(renderer, pawns);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
	SDL_RenderCopy(renderer, pawnTexture, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(pawnTexture);

	currentCellController->UpdateCells();
}

void GraphicsController::CreateSurface(bool backgroundFl) {
	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	if (backgroundFl) {
		background = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, rmask, gmask, bmask, amask);
		SDL_FillRect(background, NULL, SDL_MapRGB(background->format, 255, 255, 255));
	}
	else {
		if (pawns != nullptr) {
			SDL_FreeSurface(pawns);
			pawns = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, rmask, gmask, bmask, amask);
		}
		else {
			pawns = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, rmask, gmask, bmask, amask);
		}
	}
}

void GraphicsController::DrawGrid() {
	SDL_Rect horizontal, vertical;

	horizontal.x = 0;
	horizontal.y = 0;
	horizontal.w = 1;
	horizontal.h = SCREEN_HEIGHT;

	vertical.x = 0;
	vertical.y = 0;
	vertical.w = SCREEN_WIDTH;
	vertical.h = 1;

	for (int i = 0; i < GRID_SIZE_W; i++) {
		horizontal.x = i * GRID_CELL_S;
		SDL_FillRect(background, &horizontal, SDL_MapRGB(background->format, 0, 0, 0));
	}

	for (int i = 0; i < GRID_SIZE_H; i++) {
		vertical.y = i * GRID_CELL_S;
		SDL_FillRect(background, &vertical, SDL_MapRGB(background->format, 0, 0, 0));
	}
}

void GraphicsController::DrawPawn(std::pair<int, int> pix, int color1, int color2, int color3) {
	SDL_Rect Pawn;
	Pawn.x = pix.first;
	Pawn.y = pix.second;
	Pawn.w = GRID_CELL_S - 1;
	Pawn.h = GRID_CELL_S - 1;

	SDL_FillRect(pawns, &Pawn, SDL_MapRGB(pawns->format, color1, color2, color3));
}

void GraphicsController::DrawCells() {
	CreateSurface(false);
	for (auto c : currentCellController->Cells) {
		if (c.second->energy > 10) {
			DrawPawn(CoordsToPix(c.second->coords), c.second->generation[0], c.second->generation[1], c.second->generation[2]);
		}
		else {
			DrawPawn(CoordsToPix(c.second->coords), 0, 0, 0);
		}

	}
}

std::pair<int, int> GraphicsController::CoordsToPix(std::pair<int, int> coords) {
	coords.first = coords.first * GRID_CELL_S + 1;
	coords.second = coords.second * GRID_CELL_S + 1;
	return coords;
}
