#ifndef EMULATOR_H
#define EMULATOR_H

#include "screen.h"
#include "ines.h"
#include "cpu.h"
#include "ppu.h"

class Emulator
{
public:
    Emulator(string filePath);

    void run();
    void printPPUMemory() { ppu.printMemory(); }

private:
    CPU cpu;
    PPU ppu;
    ines rom;
    Screen screen;
};

#endif
