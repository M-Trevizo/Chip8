#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "../include/Chip8.h"

using namespace std;
using namespace CPU;
using namespace Graphics;

int main(int argc, char** argv) {

    // Check arguments for single filepath
    if(argc != 2) {
        cout << "Must give one argument containing rom filepath." << endl;
        return 1;
    }

    string path = argv[1];

    if(path.find(".ch8") == -1) {
        cout << "Rom must have file extension .ch8" << endl;
        return 1;
    }

    Chip8 chip;

    chip.loadFont();
    chip.loadRom(path);
    if(!chip.romLoaded) {
        cout << "There was a problem loading the ROM." << endl;
        return 1;
    }

    chip.display.initGraphics();
    chip.isRunning = true;

    // TODO: Get Event loop working for keyboard
    while(chip.isRunning) {

        if(SDL_PollEvent(&chip.display.event)) {
            if(chip.display.event.type == SDL_QUIT) {
                chip.display.quitGraphics();
                chip.isRunning = false;
                //break;
            }
        }

        uint16_t opCode = chip.fetch();
        array<uint8_t, 4> nibbles = chip.decode(opCode);
        chip.execute(nibbles);
        this_thread::sleep_for(chrono::microseconds(1428));     // ~700 instructions per second

    }

    return EXIT_SUCCESS;

}