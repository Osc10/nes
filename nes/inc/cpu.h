#include <cstdint>
#include <iostream>

class cpu // Implementation of the 2A03 processor
{
	uint8_t A; // Accumulator
	uint8_t X, Y; // Index Registers

	// Processor Status Flags
	// N V - B D I Z C
	// Carry Flag, Zero Flag, Interrupt Disable, Decimal Mode
	// Break Command, Overflow Flag, Negative Flag
	uint8_t N, V, B, D, I, Z, C;

	uint8_t S; // Stack Pointer
	uint16_t PC; // Program Counter

	uint8_t memory[65535]; 

	uint8_t opcode;

	uint8_t readMem();
	void writeMem(uint8_t val);
	void setZN(uint8_t val);
	uint8_t readStatus();

	void readOpcode();
	void executeInstruction();
};

