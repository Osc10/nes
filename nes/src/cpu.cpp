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

string instructionName[256] = {
	"BRK", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO",
	"PHP", "ORA", "ASL", "ANC", "NOP", "ORA", "ASL", "SLO",
	"BPL", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO",
	"CLC", "ORA", "NOP", "SLO", "NOP", "ORA", "ASL", "SLO",
	"JSR", "AND", "KIL", "RLA", "BIT", "AND", "ROL", "RLA",
	"PLP", "AND", "ROL", "ANC", "BIT", "AND", "ROL", "RLA",
	"BMI", "AND", "KIL", "RLA", "NOP", "AND", "ROL", "RLA",
	"SEC", "AND", "NOP", "RLA", "NOP", "AND", "ROL", "RLA",
	"RTI", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE",
	"PHA", "EOR", "LSR", "ALR", "JMP", "EOR", "LSR", "SRE",
	"BVC", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE",
	"CLI", "EOR", "NOP", "SRE", "NOP", "EOR", "LSR", "SRE",
	"RTS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA",
	"PLA", "ADC", "ROR", "ARR", "JMP", "ADC", "ROR", "RRA",
	"BVS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA",
	"SEI", "ADC", "NOP", "RRA", "NOP", "ADC", "ROR", "RRA",
	"NOP", "STA", "NOP", "SAX", "STY", "STA", "STX", "SAX",
	"DEY", "NOP", "TXA", "XAA", "STY", "STA", "STX", "SAX",
	"BCC", "STA", "KIL", "AHX", "STY", "STA", "STX", "SAX",
	"TYA", "STA", "TXS", "TAS", "SHY", "STA", "SHX", "AHX",
	"LDY", "LDA", "LDX", "LAX", "LDY", "LDA", "LDX", "LAX",
	"TAY", "LDA", "TAX", "LAX", "LDY", "LDA", "LDX", "LAX",
	"BCS", "LDA", "KIL", "LAX", "LDY", "LDA", "LDX", "LAX",
	"CLV", "LDA", "TSX", "LAS", "LDY", "LDA", "LDX", "LAX",
	"CPY", "CMP", "NOP", "DCP", "CPY", "CMP", "DEC", "DCP",
	"INY", "CMP", "DEX", "AXS", "CPY", "CMP", "DEC", "DCP",
	"BNE", "CMP", "KIL", "DCP", "NOP", "CMP", "DEC", "DCP",
	"CLD", "CMP", "NOP", "DCP", "NOP", "CMP", "DEC", "DCP",
	"CPX", "SBC", "NOP", "ISC", "CPX", "SBC", "INC", "ISC",
	"INX", "SBC", "NOP", "SBC", "CPX", "SBC", "INC", "ISC",
	"BEQ", "SBC", "KIL", "ISC", "NOP", "SBC", "INC", "ISC",
	"SED", "SBC", "NOP", "ISC", "NOP", "SBC", "INC", "ISC"
};

#define read16(address) ((address)[0] | ((address)[1] << 8))

void CPU::printLog(uint8_t opcode)
{
	++instructionNumber;
	cout << dec << setfill(' ') << setw(4) << instructionNumber << ": ";
	cout << hex << setw(4) << (int)PC << "  "; 
	for(int i = 0; i < 3; ++i)
	{
		cout << setw(2);
		if(i < instructionSize[opcode])
			cout << setfill('0') << (int)memory->mem[PC + i] << " ";
		else
			cout << "   ";
	}
	cout << setfill(' ') << setw(5) << instructionName[opcode];
	cout << setw(10) << "A:" << setfill('0') << setw(2) << (int)A;
	cout << setfill(' ') << setw(3) << "X:" << setfill('0') << setw(2) << (int)X;
	cout << setfill(' ') << setw(3) << "Y:" << setfill('0') << setw(2) << (int)Y;
	cout << setfill(' ') << setw(4) << "SP:" << setfill('0') << setw(2) << (int)S;
	cout << endl;
}

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
	return (N << 7) | (V << 6) | (1 << 5) | (B << 4) | (D << 3) | (I << 2) | (Z << 1) | C;
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

uint16_t CPU::readMem()
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
			return read16(&memory->mem[PC + 1]);
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
			cerr << "Invalid opcode: " << hex << (int)opcode << dec << "\n";
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

void CPU::executeInstruction()
{
	readOpcode();
#ifdef NDEBUG
	printLog(opcode);
#endif
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
				PC += (int8_t)memory->mem[PC + 1];
			break;

		//BCS
		case 0xB0:
			if(C != 0)
				PC += (int8_t)memory->mem[PC + 1];
			break;

		//BEQ
		case 0xF0:
			if(Z != 0)
				PC += (int8_t)memory->mem[PC + 1];
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
				PC += (int8_t)memory->mem[PC + 1];
			break;

		//BNE
		case 0xD0:
			if(Z == 0)
				PC += (int8_t)memory->mem[PC + 1];
			break;

		//BPL
		case 0x10:
			if(N == 0)
				PC += (int8_t)memory->mem[PC + 1];
			break;

		//BVC
		case 0x50:
			if(V == 0)
				PC += (int8_t)memory->mem[PC + 1];
			break;

		//BVS
		case 0x70:
			if(V != 0)
				PC += (int8_t)memory->mem[PC + 1];
			break;

		//CLC
		case 0x18:
			C = 0;
			break;
		
		//CLD
		case 0xD8:
			D = 0;
			break;

		//CLV
		case 0xB8:
			V = 0;
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

		//CPX
		case 0xE0:
		case 0xE4:
		case 0xEC:
			b = readMem();
			C = (X >= b) ? 1 : 0;
			setZN(X - b);
			break;

		//CPY
		case 0xC0:
		case 0xC4:
		case 0xCC:
			b = readMem();
			C = (Y >= b) ? 1 : 0;
			setZN(Y - b);
			break;

		//DEX
		case 0xCA:
			--X;
			setZN(X);
			break;
			
		//DEY
		case 0x88:
			--Y;
			setZN(Y);
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

		//INX
		case 0xE8:
			++X;
			setZN(X);
			break;
			
		//INY
		case 0xC8:
			++Y;
			setZN(Y);
			break;

		//JMP
		case 0x4C:
			PC = readMem();
			break;
		case 0x6C:
			PC = read16(&memory->mem[read16(&memory->mem[PC + 1])]);
			break;

		//JSR
		case 0x20:
			pushToStack16(PC + 2);
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

		//LDY
		case 0xA0:
		case 0xA4:
		case 0xB4:
		case 0xAC:
		case 0xBC:
			Y = readMem();
			setZN(Y);
			break;

		//NOP
		case 0xEA:
			break;

		//ORA
		case 0x09:
		case 0x05:
		case 0x15:
		case 0x0D:
		case 0x1D:
		case 0x19:
		case 0x01:
		case 0x11:
			A = A | readMem();
			setZN(A);
			break;

		//PHA
		case 0x48:
			pushToStack(A);
			break;

		//PHP
		case 0x08:
			pushToStack(readFlags() | 0x10);
			break;

		//PLA
		case 0x68:
			A = pullFromStack();
			setZN(A);
			break;	

		//PLP
		case 0x28:
			setFlags(pullFromStack());
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

		//SBC
		case 0xE9:
		case 0xE5:
		case 0xF5:
		case 0xED:
		case 0xFD:
		case 0xF9:
		case 0xE1:
		case 0xF1:
			a = A;
			b = readMem();
			c = C;

			A = a - b - (1 - c);

			C = (int(a) >= int(b) + int(1 - c)) ? 1 : 0;
			setZN(A);
			if((((a^A) & 0x80 ) != 0) && (((a^b) & 0x80) != 0)) // b, sum same sign but a has opposite sign
				V = 1;
			else
				V = 0;

			break;

		//SEC
		case 0x38:
			C = 1;
			break;

		//SED
		case 0xF8:
			D = 1;
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

		//TAX
		case 0xAA:
			X = A;
			setZN(X);
			break;

		//TAY
		case 0xA8:
			Y = A;
			setZN(Y);
			break;

		//TSX
		case 0xBA:
			X = S;
			setZN(X);
			break;

		//TXA
		case 0x8A:
			A = X;
			setZN(A);
			break;

		//TXS
		case 0x9A:
			S = X;
			break;

		//TYA
		case 0x98:
			A = Y;
			setZN(A);
			break;

		default:
			cerr << "Attempted to execute invalid opcode! \n";
			exit(0); // Remove

	}

	incrementPC(opcode);
}

void CPU::incrementPC(uint8_t opcode)
{
	switch(opcode)
	{
		case 0x4C:
		case 0x20:
		case 0x60:
			break;

		default:
			PC += instructionSize[opcode];
	}
}

void CPU::initialize()
{
	if(!memory->isLoaded)
		cerr << "CPU running while memory has not been initialized!\n";

	PC = read16(&memory->mem[0xFFFC]);
	PC = 0xC000; // Automating nestest
	S = 0xFD; // Automating nestest
}

void CPU::run()
{
	initialize();

	while(true)
	{
		executeInstruction();
	}
}
