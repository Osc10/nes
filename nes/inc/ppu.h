#ifndef PPU_H
#define PPU_H

#include <fstream>

class PPU
{
public:
    uint8_t readRegister(uint16_t address);
	void writeRegister(uint16_t address, uint8_t val);
    void executeCycle();
    void loadPatternTables(std::ifstream *inesFile, int size, int offset);

    void printMemory(); //Helper function to print state of PPU memory.
private:
    //Memory
    uint8_t ppuRegisters[0x8];
    uint8_t palettes[0x20];
    uint8_t nameTables[0x1000];
    uint8_t patternTables[0x2000];
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t val);

	int totalCycles = 0;
	int scanline = -1;
	int cycle = 0;

	void drawTileBackground(int x, int y);

	//Background
	uint8_t nameTableByte;
	uint8_t attributeTableByte;
	uint16_t tileBitmap;

	//Shift registers - Contains bitmap data for two tiles. 
	uint16_t shiftA16;
	uint16_t shiftB16;

    //Register Flags

    //$2000 - PPUCTRL
    //VPHB SINN
    //uint8_t nmiEnableFlag; //V - Generate NMI at start of VBLANK, 0:off, 1:on
    //uint8_t ppuMasterSlaveFlag; //P - 0:Read backdrop from EXT pins, 1:Output colour on EXT pins
    //uint8_t spriteHeightFlag; //H - 0:8x8, 1:8x16
    uint16_t backgroundPatternTableAddress; //B 
    //uint16_t spritePatternTableAddress; //S 
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
    //uint8_t sprite0Hit; //S 
    //uint8_t spriteOverflow; //O 
        
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
