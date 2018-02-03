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
	PPU ppu(&ppumem);

	ines rom;
	rom.load("../donkeykong.nes", &cpumem);
	//rom.load("../test.nes", &cpumem);

	cpu.initialize();

	while(true)
	{
		cpu.executeInstruction();
		ppu.executeInstruction();
	}
}
