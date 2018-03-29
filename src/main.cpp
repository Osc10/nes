#include "emulator.h"

int main()
{
	string filePath = "../donkeykong.nes";
	//"../test.nes"
	Emulator emu(filePath);

    emu.run();
	//emu.printPPUMemory();

}
