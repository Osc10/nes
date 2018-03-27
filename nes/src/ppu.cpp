#include "ppu.h"
#include "cpu.h"

uint8_t PPU::readRegister(uint16_t address)
{
    uint8_t reg = ppuRegisters[address & 0x7];
	
	switch(address & 0x7)
	{
		case 0x2:
			vblank = 0;
			//Clear address latch used by PPUSCROLL to be done.
			//Clear address latch used by PPUADDR
			ppuAddressLoaded = 0;
			//Reading at the end of VBLANK will return 0 in vblank bit but still clear 
			//address latch - to be implemented
			break;
		default:
			break;
	}

	return reg;
}

void PPU::writeRegister(uint16_t address, uint8_t val)
{
	// TODO: Does not write to $2000 in first 30000 cycles
    ppuRegisters[address & 0x7] = val;
	switch(address & 0x7)
	{
		case 0x0:
			nameTableAddress = 0x2000 + (val & 0x3)*0x400;
			incrementModeFlag = (val & (1 << 2)) >> 2;
			backgroundPatternTableAddress = ((val & (1 << 4)) == 0) ? 0x0000 : 0x1000;
			break;
		case 0x6:
			if(ppuAddressLoaded == 0 || ppuAddressLoaded == 2)
			{
				ppuAddressUpper = val;
				ppuAddressLoaded = 1;
			}
			else
			{
				ppuAddress = ((uint16_t)ppuAddressUpper << 7) | val;
				ppuAddressLoaded = 2;
			}
			break;
		case 0x7:
			if(ppuAddressLoaded == 2)
			{
                write(ppuAddress, val);
				ppuAddress = (incrementModeFlag == 0) ? ppuAddress + 1 : ppuAddress + 32;
			}
			break;
		default:
			break;
	}
}

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


