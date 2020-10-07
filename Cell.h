#pragma once
#include <utility>
#include <vector>
#include <array>

const int ADDRESS_SIZE = 256;
const int ENERGY_MIN = 5000;
const int ENERGY_MAX = 10000;

class Cell
{
private:
	int key_value;
	std::array<unsigned short, ADDRESS_SIZE> commands;
	bool Updated_flag = false;
public:
	std::array<int, 3> generation;
	int energy;
	unsigned char address = 0;
	std::pair<int, int> coords;

	unsigned char GetCommand();
	unsigned char GetAddress();
	Cell(int w_boundary, int h_boundary, bool random);
	void CopyCommands(Cell* c_Cell);
	void Mutate(int amount, int chance);
	void CalculateKey();
	int GetKey();
	bool GetUpdatedFlag();
	void SetTrueUpdatedFlag();
	void SetFalseUpdatedFlag();
};