#include "ppu.h"
#include "cpu.h"

//----------------------------------------------------------------------------------------------------
// Memory
//----------------------------------------------------------------------------------------------------
uint8_t PPU::read(uint16_t address)
{
    address = address & 0x3FFF;
    if(address < 0x2000)
        return patternTables[address];
    else if(address < 0x3F00)
        return nameTables[address & 0x0FFF]; // TODO: Implement different mirroring modes.
    else
        return palettes[address & 0x1F];
}

void PPU::write(uint16_t address, uint8_t val)
{
    address = address & 0x3FFF;
    if(address < 0x2000)
        patternTables[address] = val;
    else if(address < 0x3F00)
        nameTables[address & 0x0FFF] = val;
    else
        palettes[address & 0x1F] = val;
}

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
    uint16_t nametableSelect = (uint16_t)(val & 0x3) << 10;
    nametableAddress = 0x2000 | nametableSelect;
    assert(nametableAddress == 0x2000 || nametableAddress == 0x2400 ||
           nametableAddress == 0x2800 || nametableAddress == 0x2C00);
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
    t = (t & 0x73FF) | nametableSelect;
}

//$2001
void PPU::writePPUMASK(uint8_t val)
{

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






void PPU::executeCycle()
{
	totalCycles++;
	
	if(scanline == 261)
	{
		// TODO:A cycle is skipped if an odd frame is rendered.
		// TODO:Fill shift registers with data for the first two tiles of the next scanline.
		// TODO:For cycles 280 - 304, vertical scroll bits are reloaded if rendering enabled.
	}
	else if(scanline < 240)
	{
		if(cycle == 0)
		{
			// Dummy cycle.
		}
		else if(cycle <= 256)
		{
			// 2 cycles per memory access, and 4 bytes accessed per tile
			if(cycle % 8 == 0)
			{
				int x = cycle/8 - 1;
				int y = scanline/8;
                nameTableByte = read(nameTableAddress + x + y*32);
				x = x/4;
				y = y/4;
                attributeTableByte = read(nameTableAddress + 0x3C0 + x/4 + (y/4)*8);
            }
		}
		else if(cycle <= 320)
		{
		}
		else if(cycle <= 336)
		{
		}
		else // Cycles 337 - 340
		{
		}
	}
	else if (scanline == 240)
	{
		// Dummy scanline.
	}
	else // Scanlines 241 - 260, vblank lines where memory can be safely accessed.
	{
		if(scanline == 241 && cycle == 1) 
		{
			vblank = 1;
			// TODO: vblank NMI occurs.
		}
	}

	// There are 261 scanlines in one frame, and 341 cycles in each scanline.
	cycle = (cycle + 1)%341;
	scanline = (cycle != 0) ? scanline : (scanline + 1)%262; 
}

void PPU::loadPatternTables(std::ifstream *inesFile, int size, int offset)
{
    inesFile->read((char*)(patternTables + offset), size);
}

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


