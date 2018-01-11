#include <cstdint>
#include <string>

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

	uint8_t memory[65535]; // $0100 - $01FF Stack
						   // 
						   //
	
	uint8_t opcode;

	void readOpcode();
	void executeInstruction();
}

