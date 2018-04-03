#include "controller.h"

uint8_t Controller::readIO(uint16_t addr)
{
    if(addr % 2) // 0x4017, controller 2
        return 0x40;
    else
    {
        if(counter == 0)
            getKeyState();

        uint8_t val = 0;
        if(strobe)
            val = 0x40 | (keyPress & 0x1); // Return status of A
        else
        {
            if(counter < 8)
            {
                val = 0x40 | ((keyPress >> counter) & 0x1);
            }
            else if(counter < 24)
            {
                val = 0x40; //Second controller status ignored.
            }
            counter = (counter + 1) % 24;
        }

        return val;
    }
}

void Controller::writeIO(uint16_t addr, uint8_t val)
{
    if(addr % 2 == 0) //Write to 0x4016
    {
        strobe = (val & 0x1);
        if(strobe)
        {
            counter = 0;
        }
    }
}

enum keyPress
{
    keyA      = (1 << 0),
    keyB  	  = (1 << 1),
    keySelect = (1 << 2),
    keyStart  = (1 << 3),
    keyUp     = (1 << 4),
    keyDown   = (1 << 5),
    keyLeft   = (1 << 6),
    keyRight  = (1 << 7)
};

void Controller::getKeyState()
{
    SDL_PumpEvents();
    const uint8_t *state = SDL_GetKeyboardState(NULL);
    keyPress = 0;

    keyPress |= (state[SDL_SCANCODE_Z]) ? keyA : 0;
    keyPress |= (state[SDL_SCANCODE_X]) ? keyB : 0;
    keyPress |= (state[SDL_SCANCODE_BACKSPACE]) ? keySelect : 0;
    keyPress |= (state[SDL_SCANCODE_RETURN]) ? keyStart : 0;
    keyPress |= (state[SDL_SCANCODE_UP]) ? keyUp : 0;
    keyPress |= (state[SDL_SCANCODE_DOWN]) ? keyDown : 0;
    keyPress |= (state[SDL_SCANCODE_LEFT]) ? keyLeft : 0;
    keyPress |= (state[SDL_SCANCODE_RIGHT]) ? keyRight : 0;
}
