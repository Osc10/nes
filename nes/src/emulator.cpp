#include "emulator.h"

Emulator::Emulator(string filePath)
	: cpumem(CPUMemory()), ppumem(PPUMemory()), cpu(CPU(&cpumem)), ppu(PPU(&ppumem)), rom(ines())
{
    cpumem.setPPUMemory(&ppumem);
    rom.load(filePath, &cpumem, &ppumem);

    cpu.initialize();
}

void Emulator::run()
{
	//while(true)

	for(int i = 0; i < 300000; ++i)
	{
		cpu.executeInstruction();
		ppu.executeInstruction();
		ppu.executeInstruction();
		ppu.executeInstruction();
	}
}



