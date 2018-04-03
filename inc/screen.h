#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <SDL2/SDL.h>
#include "ppu.h"

class Screen
{
public:
    Screen(PPU *p);
    ~Screen();
    void renderFrame();
    bool quit = false;
    void pollEvents();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;
    PPU *ppu;
};

#endif
