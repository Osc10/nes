#include "emulator.h"

Emulator::Emulator(std::string filePath)
    : cpu(CPU()), ppu(PPU()), joypad(Controller()), rom(ines()), screen(Screen(&ppu))
{
    cpu.linkPPU(&ppu);
    ppu.linkCPU(&cpu);
    cpu.linkController(&joypad);
    rom.load(filePath, &cpu, &ppu);

    cpu.initialize();
}

void Emulator::run()
{
    int i = 0;
    while(!screen.quit)
    {
        ++i;


        cpu.executeCycle();
        ppu.executeCycle();
        ppu.executeCycle();
        ppu.executeCycle();
        if(ppu.postRender)
        {
            screen.pollEvents();
            screen.renderFrame();
            ppu.postRender = false;
        }
    }
}



