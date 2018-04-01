#include "emulator.h"

Emulator::Emulator(string filePath)
    : cpu(CPU()), ppu(PPU()), rom(ines()), screen(Screen(&ppu))
{
    cpu.linkPPU(&ppu);
    ppu.linkCPU(&cpu);
    rom.load(filePath, &cpu, &ppu);

    cpu.initialize();
}

void Emulator::run()
{
    while(true)
    {
        cpu.executeCycle();
        ppu.executeCycle();
        ppu.executeCycle();
        ppu.executeCycle();
        if(ppu.newFrame)
        {
            screen.renderFrame();
            ppu.newFrame = false;
        }
    }
}



