#include "cpu.h"

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

inline void CPU::setFlags(uint8_t val)
{
	C = val & 0x1;
	Z = (val >> 1) & 0x1;
	I = (val >> 2) & 0x1;
	D = (val >> 3) & 0x1;
	B = (val >> 4) & 0x1;
	V = (val >> 6) & 0x1;
	N = (val >> 7) & 0x1;
}

inline uint8_t CPU::readFlags()
{
	return (N << 7) | (V << 6) | (B << 4) | (D << 3) | (I << 2) | (Z << 1) | C;
}

inline void CPU::readOpcode()
{
	opcode = memory->mem[PC];	
}

inline void CPU::pushToStack(uint8_t val)
{
	memory->mem[0x0100 + S] = val;	
	--S;
}

inline void CPU::pushToStack16(uint16_t val)
{
	pushToStack((uint8_t)(val >> 8));
	pushToStack((uint8_t)(val & 0xFF));
}

inline uint8_t CPU::pullFromStack()
{
	++S;
	return memory->mem[0x0100 + S];
}

inline uint16_t CPU::pullFromStack16()
{
	uint16_t low = (uint16_t)pullFromStack();
	uint16_t high = (uint16_t)pullFromStack();
	return low | (high << 8);
}

inline uint8_t CPU::readStatus()
{
	uint8_t status = C | (Z << 1) | (I << 2) | (D << 3) |
					 (B << 4) | (V << 6) | (N << 7);
	return status;
}

uint8_t CPU::readMem()
{
	switch(instructionMode[opcode])
	{
		uint16_t offset, addr;
		case accumulator:
			return A;
		case immediate:
			return memory->mem[PC + 1];
		case zeroPage:
			return memory->mem[memory->mem[PC + 1]];
		case zeroPageX:
			offset = (X + memory->mem[PC + 1]) && 0xFF;
			return memory->mem[offset];
		case absolute:
			return memory->mem[read16(memory->mem + PC + 1)];
		case absoluteX:
			return memory->mem[read16(memory->mem + PC + 1) + X];
		case absoluteY:
			return memory->mem[read16(memory->mem + PC + 1) + Y];
		case indirectIndexed:
			addr = read16(memory->mem + memory->mem[PC + 1]) + Y;
			return memory->mem[addr];
		case indexedIndirect:
			offset = (X + memory->mem[PC + 1]) && 0xFF;
			return memory->mem[read16(memory->mem + offset)];
		default:
			cerr << "Invalid opcode: " << opcode << "\n";
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
			memory->mem[PC + 1] = val;
			break;
		case zeroPage:
			memory->mem[memory->mem[PC + 1]] = val;
			break;
		case zeroPageX:
			offset = (X + memory->mem[PC + 1]) && 0xFF;
			memory->mem[offset] = val;
			break;
		case absolute:
			memory->mem[read16(memory->mem + PC + 1)] = val;
			break;
		case absoluteX:
			memory->mem[read16(memory->mem + PC + 1) + X] = val;
			break;
		case absoluteY:
			memory->mem[read16(memory->mem + PC + 1) + Y] = val;
			break;
		case indirectIndexed:
			addr = read16(memory->mem + memory->mem[PC + 1]) + Y;
			memory->mem[addr] = val;
			break;
		case indexedIndirect:
			offset = (X + memory->mem[PC + 1]) && 0xFF;
			memory->mem[read16(memory->mem + offset)] = val;
			break;
		default:
			break;
	}
}

#include <iomanip>
void CPU::executeInstruction()
{
	readOpcode();
	cout << hex << (int)opcode << dec << endl; //To Delete
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
			C = ((sum >> 8) == 0) ? 0 : 1;
			setZN(A);
			if((((a^A) & 0x80 ) != 0) && (((a^b) & 0x80) == 0)) // a, b same sign but sum has opposite sign
				V = 1;
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
			C = (a & 0x80) >> 7;
			a = a << 1;
			writeMem(a);
			setZN(a);
			break;
		
		//BCC
		case 0x90:
			if(C == 0)
				PC += memory->mem[PC + 1];
			break;

		//BCS
		case 0xB0:
			if(C != 0)
				PC += memory->mem[PC + 1];
			break;

		//BEQ
		case 0xF0:
			if(Z != 0)
				PC += memory->mem[PC + 1];
			break;

		//BIT
		case 0x24:
		case 0x2C:
			b = readMem();
			Z = ((b & A) == 0) ? 1 : 0;
			V = (b & 0x40) >> 6;
			N = (b & 0x80) >> 7;
			break;
		
		//BMI
		case 0x30:
			if(N != 0)
				PC += memory->mem[PC + 1];
			break;

		//BNE
		case 0xD0:
			if(Z == 0)
				PC += memory->mem[PC + 1];
			break;

		//BPL
		case 0x10:
			if(N == 0)
				PC += memory->mem[PC + 1];
			break;
		
		//BRK
		case 0x00:
			pushToStack16(PC);
			pushToStack(readFlags());
			B = 1;
			PC = read16(&memory->mem[0xFFFE]);
			break;

		//CLD
		case 0xD8:
			D = 0;
			break;

		//CMP
		case 0xC9:
		case 0xC5:
		case 0xD5:
		case 0xCD:
		case 0xDD:
		case 0xD9:
		case 0xC1:
		case 0xD1:
			b = readMem();
			C = (A >= b) ? 1 : 0;
			setZN(A - b);
			break;

		//EOR
		case 0x49:
		case 0x45:
		case 0x55:
		case 0x4D:
		case 0x5D:
		case 0x59:
		case 0x41:
		case 0x51:
			b = readMem();
			A = A ^ b;
			setZN(A);
			break;

		//JSR
		case 0x20:
			pushToStack16(PC - 1);
			PC = read16(&memory->mem[PC + 1]);
			break;

		//LDA
		case 0xA9:
		case 0xA5:
		case 0xB5:
		case 0xAD:
		case 0xBD:
		case 0xB9:
		case 0xA1:
		case 0xB1:
			A = readMem();
			setZN(A);
			break;

		//LDX
		case 0xA2:
		case 0xA6:
		case 0xB6:
		case 0xAE:
		case 0xBE:
			X = readMem();
			setZN(X);
			break;

		//PLA
		case 0x68:
			A = pullFromStack();
			setZN(A);
			break;	

		//RTI
		case 0x40:
			setFlags(pullFromStack());
			PC = pullFromStack16();
			break;

		//RTS
		case 0x60:
			PC = pullFromStack16() + 1;
			break;

		//SEI
		case 0x78:
			I = 1;			
			break;

		//STA
		case 0x85:
		case 0x95:
		case 0x8D:
		case 0x9D:
		case 0x99:
		case 0x81:
		case 0x91:
			writeMem(A);
			break;

		//STX
		case 0x86:
		case 0x96:
		case 0x8E:
			writeMem(X);
			break;

		//TXS
		case 0x9A:
			S = X;
			break;

		default:
			cerr << "Attempted to execute invalid opcode! \n";
			exit(0); // Remove

	}

	PC += instructionSize[opcode];
}

void CPU::initialize()
{
	if(!memory->isLoaded)
		cerr << "CPU running while memory has not been initialized!\n";

	PC = read16(&memory->mem[0xFFFC]);
}

void CPU::run()
{
	initialize();

	while(true)
	{
		executeInstruction();
	}
}
