#ifndef PPU_H
#define PPU_H

class PPU
{
public:
private:

    //$2000 - PPUCTRL
    //VPHB SINN
    uint8_t nmiEnable; //V 
    uint8_t ppuMasterSlave; //P 
    uint8_t spriteHeight; //H 
    uint8_t backgroundTileSelect; //B 
    uint8_t spriteTileSelect; //S 
    uint8_t incrementMode; //I 
    uint8_t nameTableSelect; //NN

    //$2001 - PPUMASK
    //BGRs bMmG
    uint8_t colorEmphasis; //BGR
    uint8_t spriteEnable; //s 
    uint8_t backgroundEnable; //b 
    uint8_t spriteLeftColumnEnable; //M 
    uint8_t backgroundLeftColumnEnable; //m 
    uint8_t greyscale; //G 

    //$2002 - PPUSTATUS
    //VSO- ----
    uint8_t vblank; //V 
    uint8_t sprite0Hit; //S 
    uint8_t spriteOverflow; //O 
        
    //$2003 - OAMADDR
    uint8_t oamAddress;

    //$2004 - OAMDATA
    uint8_t oamData;

    //$2005 - PPUSCROLL
        
    //$2006 - PPUADDR
    uint8_t pprAddress;

    //$2007 - PPUDATA
    uint8_t pprData;

    //$4014 - OAMDMA
};
#endif
