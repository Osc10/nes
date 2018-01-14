#include <cpu.h>

enum addressingMode
{
	absolute,
	absoluteX,
	absoluteY,
	accumulator,
	immediate,
	implied,
	indexedIndirect,
	indirect,
	indirectIndexed,
	relative,
	zeroPage,
	zeroPageX,
	zeroPageY
};

uint8_t instructionMode[256] = {

	5, 6, 5, 6, 10, 10, 10, 10, 5, 4, 3, 4, 0, 0, 0, 0,
	9, 8, 5, 8, 11, 11, 11, 11, 5, 2, 5, 2, 1, 1, 1, 1,
	0, 6, 5, 6, 10, 10, 10, 10, 5, 4, 3, 4, 0, 0, 0, 0,
	9, 8, 5, 8, 11, 11, 11, 11, 5, 2, 5, 2, 1, 1, 1, 1,
	5, 6, 5, 6, 10, 10, 10, 10, 5, 4, 3, 4, 0, 0, 0, 0,
	9, 8, 5, 8, 11, 11, 11, 11, 5, 2, 5, 2, 1, 1, 1, 1,
	5, 6, 5, 6, 10, 10, 10, 10, 5, 4, 3, 4, 7, 0, 0, 0,
	9, 8, 5, 8, 11, 11, 11, 11, 5, 2, 5, 2, 1, 1, 1, 1,
	4, 6, 4, 6, 10, 10, 10, 10, 5, 4, 5, 4, 0, 0, 0, 0,
	9, 8, 5, 8, 11, 11, 12, 12, 5, 2, 5, 2, 1, 1, 2, 2,
	4, 6, 4, 6, 10, 10, 10, 10, 5, 4, 5, 4, 0, 0, 0, 0,
	9, 8, 5, 8, 11, 11, 12, 12, 5, 2, 5, 2, 1, 1, 2, 2,
	4, 6, 4, 6, 10, 10, 10, 10, 5, 4, 5, 4, 0, 0, 0, 0,
	9, 8, 5, 8, 11, 11, 11, 11, 5, 2, 5, 2, 1, 1, 1, 1,
	4, 6, 4, 6, 10, 10, 10, 10, 5, 4, 5, 4, 0, 0, 0, 0,
	9, 8, 5, 8, 11, 11, 11, 11, 5, 2, 5, 2, 1, 1, 1, 1,

};

uint8_t instructionSize[256] = {

	1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	3, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 0, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 0, 3, 0, 0,
	2, 2, 2, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,

};

#define read16(address) ((address)[0] | ((address)[1] << 8))

inline void CPU::setZN(uint8_t val)
{
	Z = (val == 0) ? 1 : 0;
	N = (val & 0x80) ? 1 : 0;
}

inline void CPU::readOpcode()
{
	opcode = memory.mem[PC];	
}

uint8_t CPU::readMem()
{
	switch(instructionMode[opcode])
	{
		uint16_t offset, addr;
		case accumulator:
			return A;
		case immediate:
			return memory.mem[PC + 1];
		case zeroPage:
			return memory.mem[memory.mem[PC + 1]];
		case zeroPageX:
			offset = (X + memory.mem[PC + 1]) && 0xFF;
			return memory.mem[offset];
		case absolute:
			return memory.mem[read16(memory.mem + PC + 1)];
		case absoluteX:
			return memory.mem[read16(memory.mem + PC + 1) + X];
		case absoluteY:
			return memory.mem[read16(memory.mem + PC + 1) + Y];
		case indirectIndexed:
			addr = read16(memory.mem + memory.mem[PC + 1]) + Y;
			return memory.mem[addr];
		case indexedIndirect:
			offset = (X + memory.mem[PC + 1]) && 0xFF;
			return memory.mem[read16(memory.mem + offset)];
		default:
			std::cerr << "Invalid opcode: " << opcode << "\n";
			return 1;
	}
}

void CPU::writeMem(uint8_t val)
{
	switch(instructionMode[opcode])
	{
		uint16_t offset, addr;
		case accumulator:
			A = val;
			break;
		case immediate:
			memory.mem[PC + 1] = val;
			break;
		case zeroPage:
			memory.mem[memory.mem[PC + 1]] = val;
			break;
		case zeroPageX:
			offset = (X + memory.mem[PC + 1]) && 0xFF;
			memory.mem[offset] = val;
			break;
		case absolute:
			memory.mem[read16(memory.mem + PC + 1)] = val;
			break;
		case absoluteX:
			memory.mem[read16(memory.mem + PC + 1) + X] = val;
			break;
		case absoluteY:
			memory.mem[read16(memory.mem + PC + 1) + Y] = val;
			break;
		case indirectIndexed:
			addr = read16(memory.mem + memory.mem[PC + 1]) + Y;
			memory.mem[addr] = val;
			break;
		case indexedIndirect:
			offset = (X + memory.mem[PC + 1]) && 0xFF;
			memory.mem[read16(memory.mem + offset)] = val;
			break;
		default:
			break;
	}
}


uint8_t CPU::readStatus()
{
	uint8_t status = C | (Z << 1) | (I << 2) | (D << 3) |
					 (B << 4) | (V << 6) | (N << 7);
	return status;
}

void CPU::executeInstruction()
{
	switch(opcode)
	{
		uint8_t a, b, c;
		uint16_t sum;

		//ADC
		case 0x69:
		case 0x65:
		case 0x75:
		case 0x6D:
		case 0x7D:
		case 0x79:
		case 0x61:
		case 0x71:
			a = A;
			b = readMem();
			c = C;

			sum = a + b + c;

			A = sum & 0xFF;
			C = (sum >> 8) ? 1 : 0;
			setZN(A);
			if(((a^N) & 0x80 ) && (((a^b) & 0x80) == 0)) // a, b positive but sum negative or
				V = 1;								   // a, b negative but sum positive
			else
				V = 0;

			break;
		
		//AND
		case 0x29:
		case 0x25:
		case 0x35:
		case 0x2D:
		case 0x3D:
		case 0x39:
		case 0x21:
		case 0x31:
			A &= readMem();
			setZN(A);
			break;

		//ASL
		case 0x0A:
		case 0x06:
		case 0x16:
		case 0x0E:
		case 0x1E:
			a = readMem();
			C = a & 0x80;
			a = a << 1;
			writeMem(a);
			setZN(a);
			break;
		
		//BCC
		case 0x90:
			if(C == 0)
				PC += memory.mem[PC + 1];
			break;

		//BCS
		case 0xB0:
			if(C != 0)
				PC += memory.mem[PC + 1];
			break;

		//BEQ
		case 0xF0:
			if(Z != 0)
				PC += memory.mem[PC + 1];
			break;

		//BIT
		case 0x24:
		case 0x2C:
			a = A & readMem();
			setZN(a);
			V = a & 0x40;
			break;
		
		//BMI
		case 0x30:
			if(N != 0)
				PC += memory.mem[PC + 1];
			break;

		//BNE
		case 0xD0:
			if(Z == 0)
				PC += memory.mem[PC + 1];
			break;

		//BPL
		case 0x10:
			if(N == 0)
				PC += memory.mem[PC + 1];
			break;

		//BRK
		case 0x00:
			B = 1;

	}
}
