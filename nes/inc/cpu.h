#ifndef CPU_H
#define CPU_H

#include <assert.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

class PPU;

class CPU //Implementation of the 2A03 processor
{
public:
    void executeCycle();
    void initialize();
    void linkPPU(PPU *p) {ppu = p;}
    void loadPRGROM(std::ifstream *inesFile, int size, int offset);
private:
    //Registers and Flags
    uint8_t A; //Accumulator
    uint8_t X, Y; //Index Registers
    uint8_t S; // Stack Pointer
    uint16_t PC; // Program Counter
    uint8_t N, V, B, D, I, Z, C; //Position: N V - B D I Z C
        //Carry Flag, Zero Flag, Interrupt Disable, Decimal Mode, Break Command, Overflow Flag, Negative Flag
    void setZN(uint8_t val);
    void setFlags(uint8_t val);
    uint8_t readFlags();

    //Memory
    uint8_t ram[0x800];
    uint8_t ioRegisters[0x20];
    uint8_t sram[0x2000];         //TODO: Implement save RAM behaviour.
    uint8_t expansionROM[0x2000]; //TODO: Implement expansion ROM behaviour.
    uint8_t programROM[0x8000];
    PPU* ppu;
    uint8_t read(uint16_t address);
    uint16_t read16(uint16_t address);
    void write(uint16_t address, uint8_t val);
    //Stack functions
    void push(uint8_t val);
    void push16(uint16_t val);
    uint8_t pop();
    uint16_t pop16();

    //Instructions
    uint8_t opcode;
    int instructionNumber = 0;
    int totalCycles = 0;
    int remainingCycles = 0;
    bool pageCrossed = false;
    uint16_t readCurrentAddress(); //Each instruction has an associated addressing mode.
                                   //This function decodes the addressing mode and returns the corresponding address.
    uint8_t readCurrentMem();	   //Reads the data stored at a specific address, dependent on the addressing mode
                                   //and instruction.
    void writeCurrentMem(uint8_t val);
    void readOpcode();
    void incrementPC(uint8_t opcode);
    void addBranchCycles(uint16_t address, uint16_t programCounter);
    bool pageCross(uint16_t addr1, uint16_t addr2);

    //Helper functions
    void printLog(uint8_t opcode);
};
#endif
