#include <iostream>
#include <fstream>
#include "../include/chip8.h"

using namespace std;
using namespace CPU;

int main(int argc, char** argv) {

    // Check arguments for single filepath
    if(argc != 2) {
        cout << "Must give one argument containing rom filepath." << endl;
        return 1;
    }

    string path = argv[1];

    chip8 chip;
    chip.loadFont();
    chip.loadRom(path);
    if(!chip.romLoaded) {
        cout << "There was a problem loading the ROM." << endl;
        return 1;
    }


    
}