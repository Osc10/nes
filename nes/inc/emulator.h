#include "ines.h"
#include "memory.h"
#include "cpu.h"
#include "ppu.h"
#include <string>

class Emulator
{
public:
	Emulator(string filePath);

	void run();
	void printPPUMemory() { ppumem.printMemory(); }

private:
	CPUMemory cpumem;
	PPUMemory ppumem;
	CPU cpu;
	PPU ppu;
	ines rom;
};
