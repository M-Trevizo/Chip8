#include <iostream>
#include <display.h>
#include <Chip8.h>

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

void Display::createPointsVector(Chip8 chip) {

    for(int height = 0; height < 32, height++) {
        for(int width = 0; width < 64; width++) {
            if(chip.display[height][width] == 1) {
                SDL_Point point = {width, height};
                pointsVector.push_back(point);
            }
        }
    }
    
}

void Display::drawVector(vector vector) {

    SDL_Point pointsArray[vector.size()];
    for(int i = 0; i < vector.size(); i++) {
        pointsArray[i] = vector[i];
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoints(renderer, pointsArray, vector.size());

}