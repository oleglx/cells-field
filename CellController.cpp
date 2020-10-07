#include "CellController.h"
#include <algorithm>

CellController::CellController(int s_wbound, int s_hbound, int s_gsize) {
	w_b = s_wbound;
	h_b = s_hbound;
	g_s = s_gsize;
}

void CellController::UpdateCell(Cell* c_Cell) {
	unsigned int action_budget = 100;
	while (action_budget) {
		unsigned char current_command = c_Cell->GetCommand();
		unsigned char current_address = c_Cell->GetAddress();
		// if current command is MOVE
		if (current_command == 0x1 && c_Cell->energy > 50) {
			Move(current_address, c_Cell);
			c_Cell->address++;
			action_budget = 0;
			c_Cell->energy -= 5;
		}
		else if (current_command == 0x2 && c_Cell->energy > 200) {
			Cell* new_cell = new Cell(w_b, h_b, false);
			Spawn(current_address, new_cell, c_Cell);
			new_cell->CopyCommands(c_Cell);
			new_cell->generation = c_Cell->generation;
			new_cell->energy = c_Cell->energy / 2;
			new_cell->Mutate(100, 1);
			c_Cell->energy /= 2;
			c_Cell->address++;
			action_budget = 0;
		}
		else if (current_command == 0x3) {
			//c_Cell->energy += 5;
			c_Cell->address++;
			current_command = c_Cell->GetCommand();
			if (current_command >= 0x3 && current_command <= 0x20) {
				c_Cell->energy += 2;
				c_Cell->address++;
				action_budget -= 5;
			}
			else if (current_command > 0x20 && current_command <= 0x25) {
				c_Cell->energy += 10;
				c_Cell->address++;
				action_budget -= 5;
			}
			action_budget -= 5;
		}
		else if (c_Cell->address != UINT8_MAX) {
			c_Cell->address++;
		}
		else {
			c_Cell->address = 0;
			c_Cell->energy -= 1;
			action_budget = 0;
		}
	}
	c_Cell->Mutate(1, 1);
	c_Cell->SetTrueUpdatedFlag();
}

void CellController::UpdateCells() {
	for (auto c : Cells) {
		if (!(c.second->GetUpdatedFlag()) && c.second->energy > 0) {
			UpdateCell(c.second);
		}
	}
	for (auto c : Cells) {
		c.second->SetFalseUpdatedFlag();
	}
	//GenerateCells(1, true);
}

void CellController::GenerateCells(int amount, bool flag_blank) {
	for (int i = 0; i < amount; i++) {
		if (flag_blank) {
			auto ptr = new Cell(w_b, h_b, false);
			int current_key = ptr->GetKey();
			if (!Cells.count(current_key)) {
				this->Cells[ptr->GetKey()] = ptr;
			}
		}
		else {
			auto ptr = new Cell(w_b, h_b, true);
			int current_key = ptr->GetKey();
			if (!Cells.count(current_key)) {
				this->Cells[ptr->GetKey()] = ptr;
			}
		}
		
	}
}

void CellController::Move(unsigned short target, Cell* current_cell) {
	float degree_per_int = 360.0f / 256;
	int sector_step = 45;
	int sector = (int)round((target * degree_per_int)) / sector_step;

	current_cell->coords.second += (sector == 5 || sector == 6) - (sector == 2 || sector == 1);
	current_cell->coords.first += (sector == 0 || sector == 7) - (sector == 3 || sector == 4);

	current_cell->coords.second = (current_cell->coords.second > h_b ? 0 : current_cell->coords.second);
	current_cell->coords.second = (current_cell->coords.second < 0 ? h_b : current_cell->coords.second);

	current_cell->coords.first = (current_cell->coords.first > w_b ? 0 : current_cell->coords.first);
	current_cell->coords.first = (current_cell->coords.first < 0 ? w_b : current_cell->coords.first);

	int previous_key = current_cell->GetKey();
	current_cell->CalculateKey();
	int current_key = current_cell->GetKey();

	if (Cells.count(current_key)) {
		current_cell->energy += Cells[current_key]->energy;
		Cells.erase(current_key);
		auto nh = Cells.extract(previous_key);
		nh.key() = current_key;
		Cells.insert(move(nh));
	}
	else {
		auto nh = Cells.extract(previous_key);
		nh.key() = current_key;
		Cells.insert(move(nh));
	}
}

void CellController::Spawn(unsigned short target, Cell* current_cell, Cell* spawn_from) {
	float degree_per_int = 360.0f / 256;
	int sector_step = 45;
	int sector = (int)round((target * degree_per_int)) / sector_step;

	current_cell->coords = spawn_from->coords;

	current_cell->coords.second += (sector == 5 || sector == 6) - (sector == 2 || sector == 1);
	current_cell->coords.first += (sector == 0 || sector == 7) - (sector == 3 || sector == 4);

	current_cell->coords.second = (current_cell->coords.second > h_b ? 0 : current_cell->coords.second);
	current_cell->coords.second = (current_cell->coords.second < 0 ? h_b : current_cell->coords.second);

	current_cell->coords.first = (current_cell->coords.first > w_b ? 0 : current_cell->coords.first);
	current_cell->coords.first = (current_cell->coords.first < 0 ? w_b : current_cell->coords.first);

	current_cell->CalculateKey();
	int current_key = current_cell->GetKey();

	if (Cells.count(current_key)) {
		current_cell->energy += Cells[current_key]->energy;
		Cells.erase(current_key);
		Cells[current_key] = current_cell;
	}
	else {
		Cells[current_key] = current_cell;
	}
}
