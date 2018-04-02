#include "emulator.h"

int main()
{
    string filePath = "../donkeykong.nes";
    //string filePath = "../mario-bros.nes";

    //string filePath = "../test.nes";
    Emulator emu(filePath);

    emu.run();
    //emu.printPPUMemory();

}
