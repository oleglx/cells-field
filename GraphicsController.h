#pragma once
#include "SDL.h"
#include "CellController.h"

class GraphicsController
{
private:
	int GRID_SIZE_W, GRID_SIZE_H, GRID_CELL_S, SCREEN_WIDTH, SCREEN_HEIGHT;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* background;
	SDL_Surface* pawns;
	SDL_Texture* backgroundTexture;
	SDL_Texture* pawnTexture;
	CellController* currentCellController;

public:
	GraphicsController(int GRID_SIZE_W, int GRID_SIZE_H, int GRID_CELL_S);
	void InitializeController();
	void InitializeBackground();
	void DrawNextFrame();
	void CreateSurface(bool backgroundFl);
	void DrawGrid();
	void DrawCells();
	void DrawPawn(std::pair<int, int> pix, int color1, int color2, int color3);
	std::pair<int, int> CoordsToPix(std::pair<int, int> coords);
};

