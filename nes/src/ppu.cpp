#include "ppu.h"
#include "cpu.h"

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
