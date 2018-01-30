#include "memory.h"

uint8_t Memory::read(uint16_t address)
{
	if(address < 0x2000)
		return ram[address & 0x7FF];
	else if(address < 0x4020)
	{
		if(address < 0x4000)
			return ppuRegisters[address & 0x7];
		else
			return ioRegisters[address & 0x1F];
	}
	else if(address < 0x6000)
		return expansionROM[address & 0x1FFF];
	else if(address < 0x8000)
		return sram[address & 0x1FFF];
	else
		return programROM[address & 0x7FFF];
}

void Memory::write(uint16_t address, uint8_t val)
{
	if(address < 0x2000)
		ram[address & 0x7FF] = val;
	else if(address < 0x4020)
	{
		if(address < 0x4000)
			ppuRegisters[address & 0x7] = val;
		else
			ioRegisters[address & 0x1F] = val;
	}
	else if(address < 0x6000)
		expansionROM[address & 0x1FFF] = val;
	else if(address < 0x8000)
		sram[address & 0x1FFF] = val;
	else
		programROM[address & 0x7FFF] = val;
}


