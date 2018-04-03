#ifndef EMULATOR_H
#define EMULATOR_H

#include "screen.h"
#include "ines.h"
#include "cpu.h"
#include "ppu.h"
#include "controller.h"

class Emulator
{
public:
    Emulator(std::string filePath);

    void run();
    void printPPUMemory() { ppu.printMemory(); }

private:
    CPU cpu;
    PPU ppu;
    Controller joypad;
    ines rom;
    Screen screen;
};

#endif
