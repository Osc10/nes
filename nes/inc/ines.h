#ifndef INES_H
#define INES_H

#include <fstream>
#include <iostream>
#include <string>
#include "memory.h"

class ines
{
public:
	void load(string path, Memory *mem);

private:
	uint8_t inesHeader[16]; 
	ifstream inesFile;
	string inesPath;
	Memory *memory;

	void loadHeader();
	void loadPrgRom();
};

#endif
