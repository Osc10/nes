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

	//while(true)
	for(int i = 0; i != 3000000; ++i)
	{
		cpu.executeInstruction();
		for(int j = 0; j != 3; ++j)
			ppu.executeInstruction();
	}

	ppumem.printMemory();
}
