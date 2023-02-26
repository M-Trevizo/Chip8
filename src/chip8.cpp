#include <iostream>
#include <fstream>
#include <stdint.h>
#include "../include/chip8.h"

using namespace std;
using namespace CPU;

void chip8::loadRom(string path) {

    ifstream stream;
    stream.open(path, ios::binary);

    if(!stream.is_open()) {
        cout << "Unable to open file." << endl;
        return;
    }

    filebuf *pBuf = stream.rdbuf();

    //TODO: Fix the loading of the rom.
    int memIndex = 0x200;
    while(pBuf->sgetc() != EOF) {
        uint8_t byte;
        uint8_t highNibble = pBuf->sbumpc();
        uint8_t lowNibble = pBuf->sbumpc();
        byte = (highNibble << 4) | lowNibble;
        mem[memIndex] = byte;
        memIndex++;
    }

    stream.close();
    romLoaded = true;

}

void chip8::loadFont() {

    int font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    int memIndex = 0x050;
    for(int i = 0; i < 80; i++) {
        mem[memIndex] = font[i];
        memIndex++;
    }

}

uint16_t chip8::fetch() {
    uint8_t highByte = mem[PC];
    PC++;
    uint8_t lowByte = mem[PC];
    PC++;
    uint16_t opCode = (highByte << 8) | lowByte;
    return opCode;
}