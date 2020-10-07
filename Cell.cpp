#include <random>
#include <utility>
#include <cmath>
#include "Cell.h"

unsigned char Cell::GetCommand() {
	unsigned short command_mask = 0xff00;
	return (commands[address] & command_mask) / 256;
}

unsigned char Cell::GetAddress() {
	unsigned short address_mask = 0x00ff;
	return (commands[address] & address_mask);
}

Cell::Cell(int w_boundary, int h_boundary, bool random) {
	if (random) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<unsigned int> commands_distrib(0, UINT16_MAX);
		std::uniform_int_distribution<unsigned int> generation_distrib(UINT8_MAX / 4, UINT8_MAX);
		std::uniform_int_distribution<int> x_distrib(0, w_boundary);
		std::uniform_int_distribution<int> y_distrib(0, h_boundary);
		std::uniform_int_distribution<int> energy_distrib(ENERGY_MIN, ENERGY_MAX);

		coords.first = x_distrib(gen);
		coords.second = y_distrib(gen);

		energy = energy_distrib(gen);

		for (int i = 0; i < ADDRESS_SIZE; i++) {
			commands[i] = commands_distrib(gen);
		}

		for (int i = 0; i < 2; i++) {
			generation[i] = generation_distrib(gen);
		}
		generation[2] = generation[1] - generation[0];
	}

	CalculateKey();
}

void Cell::Mutate(int amount, int chance) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned int> chance_dice(0, 100);
	std::uniform_int_distribution<unsigned int> commands_location(0, UINT8_MAX);
	std::uniform_int_distribution<unsigned int> commands_amount(0, UINT16_MAX);
	for (int i = 0; i < amount; i++) {
		if (chance_dice(gen) <= chance) {
			commands[commands_location(gen)] += commands_amount(gen);
		}
	}
}

void Cell::CopyCommands(Cell* c_Cell) {
	commands = c_Cell->commands;
}

void Cell::CalculateKey() {
	this->key_value = (coords.first + coords.second) * (coords.first + coords.second + 1) / 2 + coords.second;
}

int Cell::GetKey() {
	return key_value;
}

bool Cell::GetUpdatedFlag() {
	return Updated_flag;
}

void Cell::SetTrueUpdatedFlag() {
	this->Updated_flag = true;
}

void Cell::SetFalseUpdatedFlag() {
	this->Updated_flag = false;
}