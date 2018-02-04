#include "ppu.h"
#include "cpu.h"

uint8_t PPU::readRegister(uint16_t address)
{
	uint8_t reg = memory->ppuRegisters[address & 0x7];
	
	switch(address & 0x7)
	{
		case 0x2:
			vblank = 0;
			//Clear address latch used by PPUSCROLL to be done.
			//Clear address latch used by PPUADDR
			ppuAddressLoaded = 0;
			//Reading at the end of VBLANK will return 0 in vblank bit but still clear 
			//address latch - to be implemented
			break;
		default:
			break;
	}

	return reg;
}

void PPU::writeRegister(uint16_t address, uint8_t val)
{
	//Does not write to $2000 in first 30000 cycles
	memory->ppuRegisters[address & 0x7] = val;
	switch(address & 0x7)
	{
		case 0x6:
			if(ppuAddressLoaded == 0 || ppuAddressLoaded == 2)
			{
				ppuAddressUpper = val;
				ppuAddressLoaded = 1;
			}
			else
			{
				ppuAddress = ((uint16_t)ppuAddressUpper << 7) | val;
				ppuAddressLoaded = 2;
			}
			break;
		case 0x7:
			if(ppuAddressLoaded == 2)
			{
				memory->write(ppuAddress, val);
				ppuAddress = (incrementModeFlag == 0) ? ppuAddress + 1 : ppuAddress + 32;
			}
			break;
		default:
			break;
	}
}

//void readPPUCTRL()
//{
//	uint8_t flags = cpu->read(0x2000);
//	uint8_t nameTableSelect = flags & 0x3;
//	switch(nameTableSelect)
//	{
//		case 0x0:
//			nameTableAddress = 0x2000;
//			break;
//		case 0x1:
//			nameTableAddress = 0x2400;
//			break;
//		case 0x2:
//			nameTableAddress = 0x2800;
//			break;
//		case 0x3:
//			nameTableAddress = 0x2C00;
//			break;
//	}
//	incrementModeFlag = (flags & (1 << 2)) >> 2;
//	spritePatternTableAddress = ((flags & (1 << 3)) == 0) ? 0x0000 : 0x1000;
//	backgroundPatternTableAddress = ((flags & (1 << 4)) == 0) ? 0x0000 : 0x1000;
//	spriteHeightFlag = (flags & (1 << 5)) >> 5;
//	ppuMasterSlaveFlag = (flags & (1 << 6)) >> 6;
//	nmiEnableFlag = (flags & (1 << 7)) >> 7;
//}

//void readPPUSTATUS()
//{
//	vblank = 0; //vblank cleared reading PPUSTATUS - placeholder implementation
//}

void PPU::writePPUSTATUS()
{
	memory->ppuRegisters[0x2] = (vblank << 7) | (sprite0Hit << 6) | (spriteOverflow << 5);
}

void PPU::executeInstruction()
{
}
