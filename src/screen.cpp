#include "screen.h"

static const int SCREEN_WIDTH = 256;
static const int SCREEN_HEIGHT = 224;

Screen::Screen(PPU *p) : ppu(p)
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        cout << "SDL failed to initialize. SDL Error: " << SDL_GetError() << endl;
    else
    {
        window = SDL_CreateWindow( "NES Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if(window == NULL)
            cout << "Failed to create window. SDL Error: " << SDL_GetError() << endl;
        else
        {
            renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
            if(renderer == NULL)
                cout << "Failed to create renderer. SDL Error: " << SDL_GetError() << endl;
            else
            {
                texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT );
                if(texture == NULL)
                    cout << "Failed to create texture. SDL Error: " << SDL_GetError() << endl;
            }
        }
    }
}

Screen::~Screen()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_Quit();
}

/*
void Screen::renderFrame()
{
    SDL_RenderClear(renderer);
    SDL_UpdateTexture( texture, NULL, (void *) ppu->pixels, SCREEN_WIDTH * sizeof(uint32_t) );
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
*/
