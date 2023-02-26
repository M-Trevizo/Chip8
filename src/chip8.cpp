#include <iostream>
#include <fstream>
#include "chip8.h"

using namespace std;
using namespace CPU;

bool chip8::loadRom(string path) {
    bool success = false;
    ifstream stream;
    stream.open(path, ios::binary);

    if(!stream.is_open()) {
        cout << "Unable to open file." << endl;
    }

    /*
    TODO: Enter rom into memory
    */

    stream.close();
    return success;
}