#ifndef PPU_H
#define PPU_H

#include "console.h"
#include "memory.h"

class PPU
{
public:
	PPU(PPUMemory *mem) : memory(mem) { memory->setPPU(*this); }
	uint8_t readRegister(uint16_t address);
	void writeRegister(uint16_t address, uint8_t val);
	void executeInstruction();

private:
	int cycles = 0;
	PPUMemory *memory;

	void writePPUSTATUS();

    //$2000 - PPUCTRL
    //VPHB SINN
    uint8_t nmiEnableFlag; //V - Generate NMI at start of VBLANK, 0:off, 1:on
    uint8_t ppuMasterSlaveFlag; //P - 0:Read backdrop from EXT pins, 1:Output colour on EXT pins
    uint8_t spriteHeightFlag; //H - 0:8x8, 1:8x16
    uint16_t backgroundPatternTableAddress; //B 
    uint16_t spritePatternTableAddress; //S 
    uint8_t incrementModeFlag; //I - 0:horizontal, 1:vertical
    uint16_t nameTableAddress; //NN

    //$2001 - PPUMASK
    //BGRs bMmG
    //uint8_t colorEmphasis; //BGR
    //uint8_t spriteEnable; //s 
    //uint8_t backgroundEnable; //b 
    //uint8_t spriteLeftColumnEnable; //M 
    //uint8_t backgroundLeftColumnEnable; //m 
    //uint8_t greyscale; //G 

    //$2002 - PPUSTATUS
    //VSO- ----
    uint8_t vblank; //V - 0:not in vblank, 1:in vblank
    uint8_t sprite0Hit; //S 
    uint8_t spriteOverflow; //O 
        
    //$2003 - OAMADDR
    //uint8_t oamAddress;

    //$2004 - OAMDATA
    //uint8_t oamData;

    //$2005 - PPUSCROLL
        
    //$2006 - PPUADDR
    uint16_t ppuAddress;
	uint8_t ppuAddressUpper;
	uint8_t ppuAddressLower;
	uint8_t ppuAddressLoaded = 0;

    //$2007 - PPUDATA
    //uint8_t ppuData;

    //$4014 - OAMDMA
};
#endif
