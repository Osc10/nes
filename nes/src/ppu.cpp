#include "ppu.h"
#include "cpu.h"

uint8_t PPU::readRegister(uint16_t address)
{
	uint8_t reg = memory->ppuRegisters[address & 0x7];
	
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
	//Does not write to $2000 in first 30000 cycles
	memory->ppuRegisters[address & 0x7] = val;
	switch(address & 0x7)
	{
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
				memory->write(ppuAddress, val);
				ppuAddress = (incrementModeFlag == 0) ? ppuAddress + 1 : ppuAddress + 32;
			}
			break;
		default:
			break;
	}
}

void PPU::executeInstruction()
{
	totalCycles++;
	
	if(scanline == -1 || scanline == 261)
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
	scanline = (cycle != 0) ? scanline : ((scanline != 261) ? scanline + 1 : -1); 
}
