#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <SDL2/SDL.h>
#include "ppu.h"
using namespace std;

class Screen
{
public:
	Screen(PPU *consolePPU);
	~Screen();

private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	PPU *ppu;	
};

#endif
