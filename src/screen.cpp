#include "screen.h"

static const int SCREEN_WIDTH = 256;
static const int SCREEN_HEIGHT = 224;

Screen::Screen(PPU *p) : ppu(p)
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        std::cout << "SDL failed to initialize. SDL Error: " << SDL_GetError() << std::endl;
    else
    {
        window = SDL_CreateWindow( "NES Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if(window == NULL)
            std::cout << "Failed to create window. SDL Error: " << SDL_GetError() << std::endl;
        else
        {
            renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
            if(renderer == NULL)
                std::cout << "Failed to create renderer. SDL Error: " << SDL_GetError() << std::endl;
            else
            {
                texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT );
                if(texture == NULL)
                    std::cout << "Failed to create texture. SDL Error: " << SDL_GetError() << std::endl;
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


void Screen::pollEvents()
{
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    quit = true;
                break;
            default:
                break;
        }
    }
}

void Screen::renderFrame()
{
    SDL_RenderClear(renderer);
    SDL_UpdateTexture( texture, NULL, (void *)ppu->getPixelData(), SCREEN_WIDTH * sizeof(uint32_t) );
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
