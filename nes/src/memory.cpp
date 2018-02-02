#include "memory.h"

uint8_t CPUMemory::read(uint16_t address)
{
	if(address < 0x2000)
		return ram[address & 0x7FF];
	else if(address < 0x4020)
	{
		if(address < 0x4000)
			ppuMemory->readRegister(address);
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

void CPUMemory::write(uint16_t address, uint8_t val)
{
	if(address < 0x2000)
		ram[address & 0x7FF] = val;
	else if(address < 0x4020)
	{
		if(address < 0x4000)
			ppuMemory->writeRegister(address, val);
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

uint8_t PPUMemory::readRegister(uint16_t address)
{
	return ppuRegisters[address & 0x7];
}
	
void PPUMemory::writeRegister(uint16_t address, uint8_t val)
{
	ppuRegisters[address & 0x7] = val;
}

uint8_t PPUMemory::read(uint16_t address)
{
	address = address & 0x3FFF;
	if(address < 0x2000)
		return patternTables[address];
	else if(address < 0x3F00)
		return nameTables[address & 0x0FFF];
	else
		return palettes[address & 0x1F];
}

void PPUMemory::write(uint16_t address, uint8_t val)
{
	address = address & 0x3FFF;
	if(address < 0x2000)
		patternTables[address] = val;
	else if(address < 0x3F00)
		nameTables[address & 0x0FFF] = val;
	else
		palettes[address & 0x1F] = val;
}
