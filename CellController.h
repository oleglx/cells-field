#pragma once
#include "Cell.h"
#include "SDL.h"
#include <map>

class CellController
{
private:
	int w_b, h_b, g_s;
	
	void UpdateCell(Cell* c_Cell);
public:
	CellController(int s_wbound, int s_hbound, int s_gsize);
	std::map<int, Cell*> Cells;
	void UpdateCells();
	void GenerateCells(int amount, bool flag_blank);
	void Move(unsigned short target, Cell* current_cell);
	void Spawn(unsigned short target, Cell* current_cell, Cell* spawn_from);
};

