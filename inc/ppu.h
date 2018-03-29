#ifndef PPU_H
#define PPU_H

#include <fstream>
#include <assert.h>

class CPU;

class PPU
{
public:
    uint8_t readRegister(uint16_t address);
	void writeRegister(uint16_t address, uint8_t val);
    void executeCycle();
    void loadPatternTables(std::ifstream *inesFile, int size, int offset);
    void printMemory(); //Helper function to print state of PPU memory.
    void linkCPU(CPU* c) {cpu = c;}
private:
    //VRAM
    uint8_t ppuRegisters[0x8];
    uint8_t palettes[0x20];
    uint8_t nameTables[0x1000];
    uint8_t patternTables[0x2000];
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t val);

    //PPU Registers
    uint16_t v; //Current VRAM address (15 bits)
    uint16_t t; //Temporary VRAM address (15 bits)
    uint8_t x; //Fine X scroll (3 bits)
    uint8_t w = 0; //First or second write toggle, shared by PPUADDR and PPUSCROLL (1 bit)
    uint8_t f = 0; //Even or odd frame (1 bit)
    uint8_t databus = 0;

    //$2000 - PPUCTRL
    //VPHB SINN
    uint16_t nametableAddress = 0x2000; //NN - 0: 0x2000, 1: 0x2400, 2: 0x2800, 3: 0x2C00
    uint8_t addressIncrement = 1; //I - 0: add 1 (horizontal), 1: add 32 (vertical)
    uint16_t spritePatternTableAddress = 0x0000; //S - 0: 0x0000, 1: 0x1000. Ignored in 8x16 sprite mode
    uint16_t backgroundPatternTableAddress = 0x0000; //B - 0: 0x0000, 1: 0x1000.
    uint8_t spriteHeight = 8; //H - 0: 8x8, 1: 8x16
    bool vblankNMI = false; //V - switch to decide whether an NMI is generated at the start of vblank.
    void writePPUCTRL(uint8_t val);

    //$2001 - PPUMASK
    //BGRs bMmG
    bool greyscale = false; //G
    bool showBackgroundLeft = false; //m: Show background in leftmost 8 pixels of screen
    bool showSpritesLeft = false; //M: Show background in leftmost 8 pixels of screen
    bool showBackground = false; //b
    bool showSprites = false; //s
    void writePPUMASK(uint8_t val);

    //$2002 - PPUSTATUS
    bool spriteOverflow = true; //Set when >8 sprites appear on a single scanline.
    bool spriteZeroHit = false;
    bool vblank = true;
    uint8_t readPPUSTATUS();

    //$2003 - OAMADDR
    void writeOAMADDR(uint8_t val);

    //$2004 - OAMDATA
    uint8_t readOAMDATA();
    void writeOAMDATA(uint8_t val);

    //$2005 - PPUSCROLL
    void writePPUSCROLL(uint8_t val);

    //$2006 - PPUADDR
    void writePPUADDR(uint8_t val);

    //$2007 - PPUDATA
    uint8_t readPPUDATA();
    void writePPUDATA(uint8_t val);

    //$4014 - OAMDMA
    void writeOAMDMA(uint8_t val);

    //NMI Handling
    CPU *cpu;

    //Rendering
    int cycle;
    int scanline; // 341 cycles per scanline.
    uint64_t frame; // 262 scanlines per frame.
    void tick();

};
#endif
