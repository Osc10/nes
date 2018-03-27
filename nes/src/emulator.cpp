#include "emulator.h"

Emulator::Emulator(string filePath)
    : cpu(CPU()), ppu(PPU()), rom(ines()), screen(Screen(&ppu))
{
    cpu.linkPPU(&ppu);
    rom.load(filePath, &cpu, &ppu);

    cpu.initialize();
}

void Emulator::run()
{
	//while(true)

	for(int i = 0; i < 300000; ++i)
	{
        cpu.executeCycle();
	}
}



