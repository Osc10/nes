#ifndef INES_H
#define INES_H

#include <fstream>
#include <iostream>
#include <string>
#include <bitset>
#include "memory.h"

class ines
{
public:
	void load(string path, CPUMemory *mem);

private:
	uint8_t inesHeader[16]; 
	ifstream inesFile;
	string inesPath;
	CPUMemory *memory;

	void loadHeader();
	void loadPrgRom();
};

#endif
