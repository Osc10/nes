#ifndef INES_H
#define INES_H

#include <fstream>
#include <iostream>
#include <string>
#include <bitset>
#include "cpu.h"
#include "ppu.h"

class ines
{
public:
    void load(std::string path, CPU *c, PPU *p);

private:
    uint8_t inesHeader[16];
    std::ifstream inesFile;
    std::string inesPath;
    CPU* cpu;
    PPU* ppu;

    void loadHeader();
    void loadRom();
    void setMirroringMode();
};

#endif
