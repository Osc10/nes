#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <iostream>
#include <iomanip>
#include "console.h"

using namespace std;

class Memory
{
public:
	virtual uint8_t read(uint16_t address) = 0;
	virtual void write(uint16_t address, uint8_t val) = 0;
};

class PPUMemory: public Memory
{
public:
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t val);
	uint8_t readRegister(uint16_t address);
	void writeRegister(uint16_t address, uint8_t val);
	void setPPU(PPU &parentPPU) { ppu = &parentPPU; }
	uint8_t OAMDMA;
	uint8_t ppuRegisters[0x8];
	void printMemory();

private:
	PPU *ppu;
	uint8_t palettes[0x20];
	uint8_t nameTables[0x1000];
	uint8_t patternTables[0x2000];
};

class CPUMemory: public Memory
{
public:
	bool isLoaded = false;
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t val);
	uint8_t programROM[0x8000];
	void setPPUMemory(PPUMemory *mem) { ppuMemory = mem; }
private:
	PPUMemory *ppuMemory;
	uint8_t sram[0x2000];
	uint8_t expansionROM[0x2000];
	uint8_t ioRegisters[0x20];
	uint8_t ram[0x800];
};

#endif
