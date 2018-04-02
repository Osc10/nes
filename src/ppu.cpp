#include "ppu.h"
#include "cpu.h"
#include "palette.h"

//----------------------------------------------------------------------------------------------------
// PPU Memory
//----------------------------------------------------------------------------------------------------
uint8_t PPU::read(uint16_t address)
{
    address = address & 0x3FFF;
    if(address < 0x2000)
        return patternTables[address];
    else if(address < 0x3F00)
        return nameTables[mirrorAddress(address)];
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
        nameTables[mirrorAddress(address)] = val;
    else
    {
        uint8_t index = address & 0x1F;
        index = (index % 4) ? index : 0; //Every multiple of four is a copy of $3F00
        palettes[index] = val;
    }
}

uint16_t PPU::mirrorAddress(uint16_t addr)
{
    uint16_t val = addr & 0x0FFF;
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
void PPU::writePPUADDR(uint8_t val)
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
uint8_t PPU::readPPUDATA()
{
    uint8_t val = read(v);

    //Reading PPUDATA fills a read buffer.
    //Details can be found at http://wiki.nesdev.com/w/index.php/PPU_registers#The_PPUDATA_read_buffer_.28post-fetch.29
    if(v % 0x4000 < 0x3F00) // if v is not accessing palette data
    {
        uint8_t buffer = val;
        val = readBuffer;
        readBuffer = buffer;
    }
    else
    {
        readBuffer = read(v - 0x1000);
    }

    //Odd behaviour during rendering.
    //See http://wiki.nesdev.com/w/index.php/PPU_scrolling#.242007_reads_and_writes for more details
    if(scanline <= 239 || scanline == 261)
    {
        incHoriV();
        incVertV();
    }
    else
        v += addressIncrement;

    databus = val;
    return val;
}

void PPU::writePPUDATA(uint8_t val)
{
    write(v, val);

    //TODO: Odd behaviour during rendering.
    //See http://wiki.nesdev.com/w/index.php/PPU_scrolling#.242007_reads_and_writes for more details

    v += addressIncrement;
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

void PPU::incHoriV()
{
    if((v & 0x1F == 31))
    {
        v &= 0x7FE0; // Coarse X set to 0
        v ^= 0x0400; // Switch horizontal nametable
    }
    else
        v += 1;
}

void PPU::incVertV()
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
    uint16_t addr = 0x23C0 | (v & 0x0C00); //Select attribute table.
    addr |= ((v & 0x1C) >> 2) | ((v & 0x0380) >> 4); //Divide offset by 4.
    uint16_t shift = ((v & 0x40) >> 4) | (v & 0x2);
    attributeTableByte = (read(addr) >> shift) & 3;
}

void PPU::loadTileBitmapLow()
{
    uint16_t addr = ((uint16_t)nameTableByte << 4) | ((v >> 12) & 0x7) | backgroundPatternTableAddress;
    // addr = nameTableByte*16 + fine Y + backgroundPatternTableAddress
    tileBitmapLow = read(addr);
}

void PPU::loadTileBitmapHigh()
{
    uint16_t addr = ((uint16_t)nameTableByte << 4) | ((v >> 12) & 0x7) |
                    backgroundPatternTableAddress | 8;
    // addr = nameTableByte*16 + fine Y + backgroundPatternTableAddress + 8
    tileBitmapHigh = read(addr);
}

void PPU::storeTileData()
{
    uint64_t data = 0;
    for(int i = 0; i < 8; ++i)
    {
        data = data << 4;
        data |= (attributeTableByte << 2);
        data |= ((tileBitmapHigh & (1 << i)) >> i) << 1;
        data |= (tileBitmapLow & (1 << i)) >> i;
    }
    shiftRegister |= data;
}

void PPU::fetchTileData()
{
    switch(cycle % 8)
    {
        case 0:
            storeTileData();
            break;
        case 1:
            loadNameTableByte();
            break;
        case 3:
            loadAttributeTableByte();
            break;
        case 5:
            loadTileBitmapLow();
            break;
        case 7:
            loadTileBitmapHigh();
            break;
        default:
            break;
    }

}

//----------------------------------------------------------------------------------------------------
// Sprites
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
// Rendering
//----------------------------------------------------------------------------------------------------

void PPU::renderPixel()
{
    uint16_t addr = (uint16_t)(shiftRegister >> (32 + (7 - x) * 4)) & 0x4;
    uint8_t data = read(0x3F00 | addr);

    pixels[cycle - 1 + scanline*256] = paletteRGBA[data];
}

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
        newFrame = true;
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
            newFrame = true;
        }
    }
}

void PPU::executeCycle()
{
    tick();

    if(showBackground && scanline <= 239 && cycle >= 1 && cycle <= 256)
        renderPixel();

    if(scanline <= 239 || scanline == 261) // Visible Line or Pre-render line
    {
        if(showBackground)
        {
            if((cycle >= 1 && cycle <= 256) || (cycle >= 321 && cycle <= 336))
            {
                shiftRegister = shiftRegister << 4;
                fetchTileData();
                if(cycle == 256)
                    incVertV();
                else if((cycle % 8) == 0)
                    incHoriV();
            }
            else if(cycle == 257)
                setHoriV();
            else if((cycle == 337) || (cycle == 339))
                loadNameTableByte(); //Unused

            if(scanline == 261 && cycle >= 279 && cycle <= 305)
                setVertV();
        }
    }
    else if(scanline == 241 && cycle == 1)
    {
        vblank = true;
        if(vblankNMI)
            cpu->setNMI(true);
    }

    if(scanline == 261 && cycle == 1)
    {
        vblank = false;
        spriteOverflow = false;
        spriteZeroHit = false;
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


