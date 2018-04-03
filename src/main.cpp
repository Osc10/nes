#include "emulator.h"

int main()
{
    //std::string filePath = "../donkeykong.nes";
    //std::string filePath = "../mario-bros.nes";
    std::string filePath = "../contra.nes";

    //std::string filePath = "../test.nes";
    Emulator emu(filePath);

    emu.run();
    //emu.printPPUMemory();

}
