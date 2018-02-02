#include "ines.h"
#include "memory.h"
#include "cpu.h"
#include "ppu.h"

int main()
{
	CPUMemory cpumem;
	PPUMemory ppumem;
	cpumem.setPPUMemory(&ppumem);
	CPU cpu(&cpumem);

	ines rom;
	//rom.load("../donkeykong.nes", &mem);
	rom.load("../test.nes", &cpumem);

	cpu.run();
}
