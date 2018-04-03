#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>
#include <SDL2/SDL.h>

class Controller
{
public:
    uint8_t readIO(uint16_t addr);
    void writeIO(uint16_t addr, uint8_t val);

private:
    bool strobe = false;
    int counter = 0;
    //0:A, 1:B, 2:Select, 3:Start, 4:Up, 5:Down, 6:Left, 7:Right
    uint8_t keyPress = 0;
    void getKeyState();
};

#endif // CONTROLLER_H
