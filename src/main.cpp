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

    while(chip.PC < chip.mem.size()) {

        if(SDL_PollEvent(&chip.display.event)) {
            if(chip.display.event.type == SDL_QUIT) {
                chip.display.quitGraphics();
                break;
            }
        }

        uint16_t opCode = chip.fetch();
        array<uint8_t, 4> nibbles = chip.decode(opCode);
        chip.execute(nibbles);
        this_thread::sleep_for(chrono::microseconds(1400));

    }

    return EXIT_SUCCESS;

}