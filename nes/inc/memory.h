#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

using namespace std;

class Memory
{
public:
	uint8_t programROM[0x8000];
	bool isLoaded = false;
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t val);

private:
	uint8_t sram[0x2000];
	uint8_t expansionROM[0x2000];
	uint8_t ioRegisters[0x20];
	uint8_t ppuRegisters[0x8];
	uint8_t ram[0x800];
};

#endif
