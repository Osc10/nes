#ifndef PPU_H
#define PPU_H

#include <fstream>
#include <assert.h>

class CPU;

//Mirroring modes of the nameTables at $2000, $2400, $2800, $2C00. Format is:
// $2000, $2400
// $2800, $2C00
enum mirroringMode
{
    mirrorHorizontal, 	// A A
                        // B B

    mirrorVertical,   	// A B
                        // A B

    //TODO: implement mappers which can use the following mirroring modes:
    mirrorSingleScreen, // A A
                        // A A

    mirrorFourScreen    // A B
                        // C D
};

class PPU
{
public:
    uint8_t readRegister(uint16_t address);
    void writeRegister(uint16_t address, uint8_t val);
    void executeCycle();
    void loadPatternTables(std::ifstream *inesFile, int size, int offset);
    void printMemory(); //Helper function to print state of PPU memory.
    void linkCPU(CPU* c) {cpu = c;}
    void setNameTableMirroring(mirroringMode val) {mode = val;}

    uint32_t pixels[256*240] = {0};
private:
    //VRAM
    uint8_t palettes[0x20];
    uint8_t nameTables[0x1000];
    uint8_t patternTables[0x2000];
    uint8_t oamData[0x100];
    mirroringMode mode;
    uint16_t mirrorAddress(uint16_t addr);
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t val);

    //PPU Registers
    uint16_t v; //Current VRAM address (15 bits)
    uint16_t t; //Temporary VRAM address (15 bits)
    uint8_t x; //Fine X scroll (3 bits)
    uint8_t w = 0; //First or second write toggle, shared by PPUADDR and PPUSCROLL (1 bit)
    uint8_t f = 0; //Even or odd frame (1 bit)

    //Data Latches
    uint8_t databus = 0;
    uint8_t nameTableByte = 0;
    uint8_t attributeTableByte = 0;
    uint8_t tileBitmapLow = 0;
    uint8_t tileBitmapHigh = 0;
    uint64_t shiftRegister = 0;

    //$2000 - PPUCTRL
    //VPHB SINN
    //NN - 0: 0x2000, 1: 0x2400, 2: 0x2800, 3: 0x2C00
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
    bool vblank = true; //Flag switched on during vblank period.
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
    int cycle = 0;
    int scanline = 0; // 341 cycles per scanline.
    uint64_t frame = 0; // 262 scanlines per frame.
    void loadNameTableByte();
    void loadAttributeTableByte();
    void loadTileBitmapLow();
    void loadTileBitmapHigh();
    void setHoriV();
    void setVertV();
    void incHoriV();
    void incVertV();
    void storeTileData();
    void fetchTileData();
    void tick();
    void renderPixel();
};
#endif
