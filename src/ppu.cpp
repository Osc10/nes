#include "ppu.h"
#include "cpu.h"

//----------------------------------------------------------------------------------------------------
// PPU Memory
//----------------------------------------------------------------------------------------------------
uint8_t PPU::read(uint16_t address)
{
    address = address & 0x3FFF;
    if(address < 0x2000)
        return patternTables[address];
    else if(address < 0x3F00)
        return nameTables[mirrorAddress()];
    else
    {
        uint8_t index = address & 0x1F;
        index = (index % 4) ? index : 0; //Every multiple of four is a copy of $3F00
        return palettes[index];
    }
}

void PPU::write(uint16_t address, uint8_t val)
{
    address = address & 0x3FFF;
    if(address < 0x2000)
        patternTables[address] = val;
    else if(address < 0x3F00)
        nameTables[mirrorAddress()] = val;
    else
    {
        uint8_t index = address & 0x1F;
        index = (index % 4) ? index : 0; //Every multiple of four is a copy of $3F00
        palettes[index] = val;
    }
}

uint16_t PPU::mirrorAddress(uint16_t addr)
{
    uint16_t val = address & 0x0FFF;
    switch(mode)
    {
        case mirrorHorizontal:
            val &= 0xBFF; //$2000 = $2400, $2800 = $2C00
            break;
        case mirrorVertical:
            val &= 0x7FF; //$2000 = $2800, $2400 = $2C00
            break;
        case mirrorSingleScreen:
            val &= 0x3FF; //$2000 = $2400 = $2800 = $2C00
            break;
        default:
            //mirrorFourScreen: return val
            break;
    }
    return val;
}

void PPU::loadPatternTables(std::ifstream *inesFile, int size, int offset)
{
    inesFile->read((char*)(patternTables + offset), size);
}

//----------------------------------------------------------------------------------------------------
// IO Registers
//----------------------------------------------------------------------------------------------------
uint8_t PPU::readRegister(uint16_t address)
{
    if(address == 0x4014)
        return databus;

    switch(address & 0x7)
    {
        case 0x2:
            return readPPUSTATUS();
        case 0x4:
            return readOAMDATA();
        case 0x7:
            return readPPUDATA();
        default:
            return databus;
    }
}

void PPU::writeRegister(uint16_t address, uint8_t val)
{
    databus = val;

    if(address == 0x4014)
    {
        writeOAMDMA(val);
        return;
    }

    // TODO: Does not write to $2000 in first 30000 cycles
    switch(address & 0x7)
    {
        case 0x0:
            writePPUCTRL(val);
            break;
        case 0x1:
            writePPUMASK(val);
            break;
        case 0x4:
            writeOAMADDR(val);
            break;
        case 0x5:
            writePPUSCROLL(val);
            break;
        case 0x6:
            writePPUADDR(val);
        break;
        case 0x7:
            writePPUDATA(val);
            break;
        default:
            break;
    }
}

//$2000
void PPU::writePPUCTRL(uint8_t val)
{
    addressIncrement = (val & (1 << 2)) ? 32 : 1;
    spritePatternTableAddress = (uint16_t)(val & (1 << 3)) << 9;
    assert(spritePatternTableAddress == 0x0000 || spritePatternTableAddress == 0x1000);
    backgroundPatternTableAddress = (uint16_t)(val & (1 << 4)) << 8;
    assert(backgroundPatternTableAddress == 0x0000 || backgroundPatternTableAddress == 0x1000);
    spriteHeight = (val & (1 << 5)) ? 16 : 8;

    bool activated = (val & (1 << 7));
    if(!vblankNMI && activated && vblank) //If vblankNMI is turned on during vblank, NMI is generated.
        cpu->setNMI(true);
    vblankNMI = (bool)(val & (1 << 7));

    // t: ...BA.. ........ = d: ......BA
    t = (t & 0x73FF) | ((uint16_t)(val & 0x3) << 10);
}

//$2001
void PPU::writePPUMASK(uint8_t val)
{
    greyscale = val & 0x1;
    showBackgroundLeft = val & (1 << 1);
    showSpritesLeft = val & (1 << 2);
    showBackground = val & (1 << 3);
    showSprites = val & (1 << 4);
}

//$2002
uint8_t PPU::readPPUSTATUS()
{
    uint8_t val = ((uint8_t)spriteOverflow << 5) | ((uint8_t)spriteZeroHit << 6) | ((uint8_t)vblank << 7);
    val |= (databus & 0x1F);
    databus = val;

    vblank = false;
    w = 0;
    return val;
}

//$2003
void PPU::writeOAMADDR(uint8_t val)
{

}

//$2004
uint8_t PPU::readOAMDATA()
{
    uint8_t val;
    databus = val;
    return val;
}

void PPU::writeOAMDATA(uint8_t val)
{

}

//$2005
void PPU::writePPUSCROLL(uint8_t val)
{
    if(w == 0)
    {
        // t: ....... ...HGFED = d: HGFED...
        t = (t & 0x7FE0) & (uint16_t)(val >> 3);
        // x:              CBA = d: .....CBA
        x = val & 0x7;
        w = 1;
    }
    else
    {
        // t: CBA..HG FED..... = d: HGFEDCBA
        t = (t & 0x0FFF) | ((uint16_t)(val & 0x7) << 12);
        t = (t & 0x7C1F) | ((uint16_t)(val & 0xF8) << 2);
        w = 0;
    }
}

//$2006
void PPU::writePPUAADR(uint8_t val)
{
    if(w == 0)
    {
        // t: .FEDCBA ........ = d: ..FEDCBA
        // t: X...... ........ = 0
        t = (t & 0x00FF) | ((uint16_t)(val & 0x3F) << 8);
        w = 1;
    }
    else
    {
        // t: ....... HGFEDCBA = d: HGFEDCBA
        // v                   = t
        t = (t & 0x7F00) | (uint16_t)val;
        v = t;
        w = 0;
    }
}

//$2007

//Read PPU Scrolling - v updates differently during rendering
uint8_t PPU::readPPUSTATUS()
{
    uint8_t val;
    databus = val;
    return val;
}

void PPU::writePPUSTATUS(uint8_t val)
{

}

//$4014
void PPU::writeOAMDMA(uint8_t val)
{

}

//----------------------------------------------------------------------------------------------------
// Background
//----------------------------------------------------------------------------------------------------
void PPU::setHoriV()
{
    //v: ....F.. ...EDCBA = t: ....F.. ...EDCBA
    v = (v & 0x7BE0) | (t & 0x041F);
}

void PPU::setVertV()
{
    //v: IHGF.ED CBA..... = t: IHGF.ED CBA.....
    v = (v & 0x041F) | (t & 0x7BE0);
}

void PPU::incCoarseX()
{
    if((v & 0x1F == 31))
    {
        v &= 0x7FE0; // Coarse X set to 0
        v ^= 0x0400; // Switch horizontal nametable
    }
    else
        v += 1;
}

void PPU::incFineY()
{
    if(~(v & 0x7000)) // if fine Y < 7
        v += 0x1000;
    else
    {
        if((v & 0x03E0) == 0x03A0) // if coarse Y = 29
        {
            v &= 0x0C1F; // fine Y = 0, coarse Y = 0
            v ^= 0x0800; // Switch vertical nametable
        }
        else if((v & 0x03E0) == 0x03E0) // if coarse Y = 31
            v &= 0x0C1F; // fine Y = 0, coarse Y = 0
        else
        {
            v &= 0x0FFF; // fine Y = 0
            v += 0x0020; // increment coarse Y
        }
    }
}

void PPU::loadNameTableByte()
{
    nameTableByte = read(0x2000 | (v & 0x0FFF));
}

void PPU::loadAttributeTableByte()
{

}

void PPU::loadTileBitmapLow()
{

}

void PPU::loadTileBitmapHigh()
{

}

//----------------------------------------------------------------------------------------------------
// Sprites
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
// Rendering
//----------------------------------------------------------------------------------------------------
void PPU::tick()
{
    cycle++;

    //Odd frames are one cycle shorter with rendering enabled
    if(cycle == 340 && scanline == 261 && f == 1 && (showBackground || showSprites))
    {
        cycle = 0;
        scanline = 0;
        frame++;
        f ^= 1;
    }
    else if(cycle > 340)
    {
        cycle = 0;
        scanline++;
        if(scanline > 261)
        {
            scanline = 0;
            frame++;
            f ^= 1;
        }
    }
}

void PPU::executeCycle()
{
    tick();

    if(showBackground)
    {
        if(scanline <= 239) //Visible scanlines.
        {
            if(cycle > 0) //Cycle 0 idle.
            {
                if(cycle <= 256)
                {

                }
                else if(cycle <= 320)
                {

                }
                else if(cycle <= 336)
                {

                }
                else //Cycles 337-340
                {

                }
            }

        }//PPU idles during scanline 240.
        else if(scanline == 241) //Vertical blanking lines: 241 - 260
        {
            if(cycle == 1) //vblank flag set on the second tick of scanline 241.
            {
                vblank = true;
                if(vblankNMI)
                    cpu->setNMI(true);
            }
        }
        else if(scanline == 261) //Pre-render scanline
        {

        }
    }
}

//----------------------------------------------------------------------------------------------------
// Other
//----------------------------------------------------------------------------------------------------
void PPU::printMemory()
{
    for(int i = 0; i < 0x400; ++i)
    {
        std::cout << std::hex << std::setfill('0') << std::setw(4) << (int)i*16 << ": ";
        for(int j = 0; j < 16; ++j)
        {
            std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)read(16 * i + j) << " ";
        }
        std::cout << std::dec << std::endl;
    }
}


