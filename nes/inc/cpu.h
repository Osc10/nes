#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <iomanip>
#include <string>
#include "memory.h"

class CPU // Implementation of the 2A03 processor
{
public:
	CPU(CPUMemory *mem) : memory(mem) {}
	void run();
private:
	CPUMemory *memory;
	uint8_t A; // Accumulator
	uint8_t X, Y; // Index Registers

	// Processor Status Flags
	// N V - B D I Z C
	// Carry Flag, Zero Flag, Interrupt Disable, Decimal Mode
	// Break Command, Overflow Flag, Negative Flag
	uint8_t N, V, B, D, I, Z, C;

	uint8_t S; // Stack Pointer
	uint16_t PC; // Program Counter

	uint8_t opcode;

	int instructionNumber = 0;

	uint8_t read(uint16_t address) { return memory->read(address); }
	void write(uint16_t address, uint8_t val) { memory->write(address, val); }

	uint16_t readCurrentAddress();
	uint8_t readCurrentMem();
	void writeCurrentMem(uint8_t val);
	void setZN(uint8_t val);
	void setFlags(uint8_t val);
	uint8_t readFlags();
	uint8_t readStatus();
	void pushToStack(uint8_t val);
	void pushToStack16(uint16_t val);
	uint8_t pullFromStack();
	uint16_t pullFromStack16();

	void readOpcode();
	void incrementPC(uint8_t opcode);
	void executeInstruction();

	void printLog(uint8_t opcode);
	
	void initialize();
};
#endif
