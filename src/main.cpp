#include "emulator.h"

int main()
{
    //std::string filePath = "../donkeykong.nes";
    //std::string filePath = "../mario-bros.nes";
    //std::string filePath = "../super-mario.nes";

    //std::string filePath = "../kirby.nes";
    //std::string filePath = "../metroid.nes";
    //std::string filePath = "../zelda.nes";
    //std::string filePath = "../castlevania.nes";
    //std::string filePath = "../contra.nes";
    //std::string filePath = "../final-fantasy.nes";

    std::string filePath = "../test.nes";
    Emulator emu(filePath);

    emu.run();
    //emu.printPPUMemory();

}
