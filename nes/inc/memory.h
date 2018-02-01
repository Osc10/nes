#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

using namespace std;

class Memory
{
public:
	bool isLoaded = false;
	virtual uint8_t read(uint16_t address) = 0;
	virtual void write(uint16_t address, uint8_t val) = 0;
};

class CPUMemory: public Memory
{
public:
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t val);
	uint8_t programROM[0x8000];
private:
	uint8_t sram[0x2000];
	uint8_t expansionROM[0x2000];
	uint8_t ioRegisters[0x20];
	uint8_t ppuRegisters[0x8];
	uint8_t ram[0x800];
};

class PPUMemory: public Memory
{
public:
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t val);

private:
	uint8_t palettes[0x20];
	uint8_t nameTables[0x1000];
	uint8_t patternTables[0x2000];
};

#endif
