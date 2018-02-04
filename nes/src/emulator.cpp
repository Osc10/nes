#include "emulator.h"

Emulator::Emulator(string filePath)
	: cpumem(CPUMemory()), ppumem(PPUMemory()), cpu(CPU(&cpumem)), ppu(PPU(&ppumem)), rom(ines())
{
    cpumem.setPPUMemory(&ppumem);
    rom.load(filePath, &cpumem);

    cpu.initialize();
}

void Emulator::run()
{
	for(int i = 0; i != 60; ++i)
		renderFrame();	
}

void Emulator::renderFrame()
{
	for(int i = 0; i != 10000; ++i)
	{
		cpu.executeInstruction();
		ppu.executeInstruction();
		ppu.executeInstruction();
		ppu.executeInstruction();
	}
}


