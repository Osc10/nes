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
	void load(string path, CPUMemory *mem, PPUMemory *ppumem);

private:
	uint8_t inesHeader[16]; 
	ifstream inesFile;
	string inesPath;
	CPUMemory *memory;
	PPUMemory *ppumemory;

	void loadHeader();
	void loadRom();
};

#endif
