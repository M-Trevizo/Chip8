#include <iostream>
#include <display.h>

using namespace Graphics;

void Display::initGraphics() {
    
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_Window("MiChip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT);

    if(NULL == window) {
        cout << "Coult not create window: " << SDL_GetError() << endl;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, 64, 32);

}

void Display::quitGraphics() {

    if(renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }

    if(window != NULL) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
    
}