#include "cpu.h"
#include "ppu.h"

//----------------------------------------------------------------------------------------------------
// Flags
//----------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------
// Memory
//----------------------------------------------------------------------------------------------------
uint8_t CPU::read(uint16_t address)
{
    if(address < 0x2000)
        return ram[address & 0x7FF];
    else if(address < 0x4020)
    {
        if(address < 0x4000 || address == 0x4014)
            ppu->readRegister(address);
        else
            return ioRegisters[address & 0x1F]; //Placeholder
    }
    else if(address < 0x6000)
        return expansionROM[address & 0x1FFF]; //Placeholder
    else if(address < 0x8000)
        return sram[address & 0x1FFF]; //Placeholder
    else
        return programROM[address & 0x7FFF];
}

uint16_t CPU::read16(uint16_t address)
{
    return (uint16_t)read(address) | ((uint16_t)read(address + 1) << 8);
}

void CPU::write(uint16_t address, uint8_t val)
{
    if(address < 0x2000)
        ram[address & 0x7FF] = val;
    else if(address < 0x4020)
    {
        if(address < 0x4000 || address == 0x4014)
            ppu->writeRegister(address, val);
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

void CPU::loadPRGROM(std::ifstream *inesFile, int size, int offset)
{
    inesFile->read((char*)(programROM + offset), size);
}

//----------------------------------------------------------------------------------------------------
// Stack
//----------------------------------------------------------------------------------------------------
inline void CPU::push(uint8_t val)
{
    write(0x0100 + S, val);
    --S;
}

inline void CPU::push16(uint16_t val)
{
    push((uint8_t)(val >> 8));
    push((uint8_t)(val & 0xFF));
}

inline uint8_t CPU::pop()
{
    ++S;
    return read(0x0100 + S);
}

inline uint16_t CPU::pop16()
{
    uint16_t low = (uint16_t)pop();
    uint16_t high = (uint16_t)pop();
    return low | (high << 8);
}

//----------------------------------------------------------------------------------------------------
// Instructions
//----------------------------------------------------------------------------------------------------
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

//Lookup table for the addressingMode of each opcode
constexpr uint8_t instructionMode[256] =
{
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

constexpr uint8_t instructionSize[256] =
{
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

const std::string instructionName[256] =
{
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

constexpr uint8_t instructionCycles[256] =
{
	7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
	2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
	2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
	2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
	2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
	2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
	2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
	2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
	2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
	2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
	2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
};

//Certain instructions increase in cycle length if certain memory wrap arounds occur.
constexpr uint8_t instructionPageCrossedCycles[256] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
};

inline void CPU::readOpcode()
{
    opcode = read(PC);
}

inline bool CPU::pageCross(uint16_t addr1, uint16_t addr2)
{
	return (addr1 & 0xFF00) != (addr2 & 0xFF00);
}

//Add 1 to cycle if branch succeeds, and add 2 if it branches to a new page
inline void CPU::addBranchCycles(uint16_t address, uint16_t programCounter)
{
    ++remainingCycles;
	if(pageCross(address, programCounter + 2))
        ++remainingCycles;
}

uint16_t CPU::readCurrentAddress()
{
	switch(instructionMode[opcode])
	{
		uint8_t offset;
		uint16_t addr;
		case immediate:
			return PC + 1;
		case zeroPage:
            return read(PC + 1);
		case zeroPageX:
            return (X + read(PC + 1)) & 0xFF;
		case zeroPageY:
            return (Y + read(PC + 1)) & 0xFF;
		case absolute:
			return read16(PC + 1); 
		case absoluteX:
			addr = read16(PC + 1);
			pageCrossed = pageCross(addr, addr + X);
			return addr + X;
		case absoluteY:
			addr = read16(PC + 1);
			pageCrossed = pageCross(addr, addr + Y);
			return addr + Y;
		case indirectIndexed:
            offset = read(PC + 1);
			if(offset != 0xFF)
                addr = read16(read(PC + 1));
			else
                addr = (read(0) << 8) | read(0xFF);
			pageCrossed = pageCross(addr, addr + Y);
			return addr + Y;
		case indexedIndirect:
            offset = (X + read(PC + 1)) & 0xFF;
			if(offset != 0xFF)	
				return read16(offset);
			else
                return ((read(0) << 8) | read(0xFF));
		case indirect:
			addr = read16(PC + 1);
			if((addr & 0xFF) != 0xFF)
				return read16(read16(PC + 1));
			else
                return (read(addr & 0xFF00) << 8) | read(addr);
		default:
			return 0;
	}
}

uint8_t CPU::readCurrentMem()
{
    if(instructionMode[opcode] == accumulator)
        return A;
    else
        return read(readCurrentAddress());
}

void CPU::writeCurrentMem(uint8_t val)
{
    if(instructionMode[opcode] == accumulator)
        A = val;
    else
        write(readCurrentAddress(), val);
}

void CPU::executeCycle()
{
#ifndef NDEBUG
	totalCycles++;
#endif
    remainingCycles--;
    if(remainingCycles >= 0)
		return;
	readOpcode();
#ifndef NDEBUG
	printLog(opcode);
#endif

    //Interrupt handling
    if(nmiFlag)
        nmi();
    else if(irqFlag && !I)
        irq();

    switch(opcode)
	{
		uint8_t a, b, c;
		uint16_t sum;
		uint16_t address;

		//ADC
        case 0x69: case 0x65: case 0x75: case 0x6D: case 0x7D: case 0x79: case 0x61: case 0x71:
			a = A;
			b = readCurrentMem();
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
        case 0x29: case 0x25: case 0x35: case 0x2D: case 0x3D: case 0x39: case 0x21: case 0x31:
			A &= readCurrentMem();
			setZN(A);
			break;

		//ASL
        case 0x0A: case 0x06: case 0x16: case 0x0E: case 0x1E:
			a = readCurrentMem();
			C = (a & 0x80) >> 7;
			a = a << 1;
			writeCurrentMem(a);
			setZN(a);
			break;
		
		//BCC
		case 0x90:
			if(C == 0)
			{
                address = PC + (int8_t)read(PC + 1);
				addBranchCycles(address, PC);
				PC = address;
			}
			break;

		//BCS
		case 0xB0:
			if(C != 0)
			{
                address = PC + (int8_t)read(PC + 1);
				addBranchCycles(address, PC);
				PC = address;
			}
			break;

		//BEQ
		case 0xF0:
			if(Z != 0)
			{
                address = PC + (int8_t)read(PC + 1);
				addBranchCycles(address, PC);
				PC = address;
			}
			break;

		//BIT
        case 0x24: case 0x2C:
			b = readCurrentMem();
			Z = ((b & A) == 0) ? 1 : 0;
			V = (b & 0x40) >> 6;
			N = (b & 0x80) >> 7;
			break;
		
		//BMI
		case 0x30:
			if(N != 0)
			{
                address = PC + (int8_t)read(PC + 1);
				addBranchCycles(address, PC);
				PC = address;
			}
			break;

		//BNE
		case 0xD0:
			if(Z == 0)
			{
                address = PC + (int8_t)read(PC + 1);
				addBranchCycles(address, PC);
				PC = address;
			}
			break;

		//BPL
		case 0x10:
			if(N == 0)
			{
                address = PC + (int8_t)read(PC + 1);
				addBranchCycles(address, PC);
				PC = address;
			}
			break;

        //BRK
        case 0x00:
            push16(PC + 2);
            push(readFlags() | 0x04); //Set B to 1
            I = 1;
            PC = read16(0xFFFE);
            break;

		//BVC
		case 0x50:
			if(V == 0)
			{
                address = PC + (int8_t)read(PC + 1);
				addBranchCycles(address, PC);
				PC = address;
			}
			break;

		//BVS
		case 0x70:
			if(V != 0)
			{
                address = PC + (int8_t)read(PC + 1);
				addBranchCycles(address, PC);
				PC = address;
			}
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
        case 0xC9: case 0xC5: case 0xD5: case 0xCD: case 0xDD: case 0xD9: case 0xC1: case 0xD1:
			b = readCurrentMem();
			C = (A >= b) ? 1 : 0;
			setZN(A - b);
			break;

		//CPX
        case 0xE0: case 0xE4: case 0xEC:
			b = readCurrentMem();
			C = (X >= b) ? 1 : 0;
			setZN(X - b);
			break;

		//CPY
        case 0xC0: case 0xC4: case 0xCC:
			b = readCurrentMem();
			C = (Y >= b) ? 1 : 0;
			setZN(Y - b);
			break;

		//DEC
        case 0xC6: case 0xD6: case 0xCE: case 0xDE:
			a = readCurrentMem() - 1;
			setZN(a);
			writeCurrentMem(a);
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
        case 0x49: case 0x45: case 0x55: case 0x4D: case 0x5D: case 0x59: case 0x41: case 0x51:
			b = readCurrentMem();
			A = A ^ b;
			setZN(A);
			break;

		//INC
        case 0xE6: case 0xF6: case 0xEE: case 0xFE:
			a = readCurrentMem() + 1;
			setZN(a);
			writeCurrentMem(a);
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
        case 0x4C: case 0x6C:
			PC = readCurrentAddress();
			break;

		//JSR
		case 0x20:
            push16(PC + 2);
			PC = read16(PC + 1);
			break;

		//LDA
        case 0xA9: case 0xA5: case 0xB5: case 0xAD: case 0xBD: case 0xB9: case 0xA1: case 0xB1:
			A = readCurrentMem();
			setZN(A);
			break;

		//LDX
        case 0xA2: case 0xA6: case 0xB6: case 0xAE: case 0xBE:
			X = readCurrentMem();
			setZN(X);
			break;

		//LDY
        case 0xA0: case 0xA4: case 0xB4: case 0xAC: case 0xBC:
			Y = readCurrentMem();
			setZN(Y);
			break;

		//LSR
        case 0x4A: case 0x46: case 0x56: case 0x4E: case 0x5E:
			a = readCurrentMem();
			C = a & 0x1;
			a = a >> 1;
			writeCurrentMem(a);
			setZN(a);
			break;

		//NOP
		case 0xEA:
			break;

		//ORA
        case 0x09: case 0x05: case 0x15: case 0x0D: case 0x1D: case 0x19: case 0x01: case 0x11:
			A = A | readCurrentMem();
			setZN(A);
			break;

		//PHA
		case 0x48:
            push(A);
			break;

		//PHP
		case 0x08:
            push(readFlags() | 0x10); //Set B to 1
			break;

		//PLA
		case 0x68:
            A = pop();
			setZN(A);
			break;	

		//PLP
		case 0x28:
            setFlags(pop());
			break;

		//ROL
        case 0x2A: case 0x26: case 0x36: case 0x2E: case 0x3E:
			a = readCurrentMem();
			c = a & (1 << 7);
			a = (a << 1) | C;
			C = c;
			writeCurrentMem(a);
			setZN(a);
			break;

		//ROR
        case 0x6A: case 0x66: case 0x76: case 0x6E: case 0x7E:
			a = readCurrentMem();
			c = a & 0x1;
			a = (a >> 1) | (C << 7);
			C = c;
			writeCurrentMem(a);
			setZN(a);
			break;

		//RTI
		case 0x40:
            setFlags(pop());
            PC = pop16();
			break;

		//RTS
		case 0x60:
            PC = pop16() + 1;
			break;

		//SBC
        case 0xE9: case 0xE5: case 0xF5: case 0xED: case 0xFD: case 0xF9: case 0xE1: case 0xF1:
			a = A;
			b = readCurrentMem();
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
        case 0x85: case 0x95: case 0x8D: case 0x9D: case 0x99: case 0x81: case 0x91:
			writeCurrentMem(A);
			break;

		//STX
        case 0x86: case 0x96: case 0x8E:
			writeCurrentMem(X);
			break;

		//STY
        case 0x84: case 0x94: case 0x8C:
			writeCurrentMem(Y);
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
            std::cerr << "Attempted to execute invalid opcode! \n";
			exit(0); // Remove

	}

    remainingCycles += instructionCycles[opcode];
	if(pageCrossed)
	{
        remainingCycles += instructionPageCrossedCycles[opcode];
		pageCrossed = false;
	}
	incrementPC(opcode);
}

void CPU::incrementPC(uint8_t opcode)
{
	switch(opcode)
    {
        case 0x00: case 0x40: case 0x4C: case 0x20: case 0x60: case 0x6C:
			break;

		default:
			PC += instructionSize[opcode];
	}
}

//----------------------------------------------------------------------------------------------------
// Interrupts
//----------------------------------------------------------------------------------------------------
void CPU::nmi()
{
    push16(PC);
    push(readFlags());
    I = 1; //Prevent further interrupts.
    PC = read16(0xFFFA);
    remainingCycles += 7;
}

void CPU::irq()
{
    push16(PC);
    push(readFlags());
    I = 1; //Prevent further interrupts.
    PC = read16(0xFFFE);
    remainingCycles += 7;
}

//----------------------------------------------------------------------------------------------------
// Other
//----------------------------------------------------------------------------------------------------
void CPU::initialize()
{
    S = 0xFD;
    setFlags(0x4); //IRQ disabled
    PC = read16(0xFFFC);
    //PC = 0xC000; // Automating nestest
}

void CPU::printLog(uint8_t opcode)
{
    ++instructionNumber;
    std::cout << std::dec << std::setfill(' ') << std::setw(4) << instructionNumber << ": ";
    std::cout << std::hex << std::setw(4) << (int)PC << "  ";
    for(int i = 0; i < 3; ++i)
    {
        std::cout << std::setw(2);
        if(i < instructionSize[opcode])
            std::cout << std::setfill('0') << (int)read(PC + i) << " ";
        else
            std::cout << "   ";
    }
    std::cout << std::setfill(' ') << std::setw(5) << instructionName[opcode];
    std::cout << std::setw(10) << "A:" << std::setfill('0') << std::setw(2) << (int)A;
    std::cout << std::setfill(' ') << std::setw(3) << "X:" << std::setfill('0') << std::setw(2) << (int)X;
    std::cout << std::setfill(' ') << std::setw(3) << "Y:" << std::setfill('0') << std::setw(2) << (int)Y;
    std::cout << std::setfill(' ') << std::setw(4) << "SP:" << std::setfill('0') << std::setw(2) << (int)S;
    std::cout << std::setfill(' ') << std::setw(5) << "CYC:" << std::setfill(' ') << std::dec << std::setw(3) << ((totalCycles - 1) * 3) % 341;
    std::cout << std::endl;
}
