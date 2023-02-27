#include <iostream>
#include <fstream>
#include <stdint.h>
#include <array>
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

array<uint8_t, 4> chip8::decode(uint16_t opCode) {

    uint8_t nibble1;
    uint8_t nibble2;
    uint8_t nibble3;
    uint8_t nibble4;

    uint16_t mask1 = 0xF000;
    uint16_t mask2 = 0x0F00;
    uint16_t mask3 = 0x00F0;
    uint16_t mask4 = 0x000F;

    nibble1 = opCode & mask1;
    nibble2 = opCode & mask2;
    nibble3 = opCode & mask3;
    nibble4 = opCode & mask4;

    array<uint8_t, 4> nibbles = {
        nibble1,
        nibble2,
        nibble3,
        nibble4
    };

    return nibbles;

}

void chip8::execute(array<uint8_t, 4> nibbles) {
    
    uint8_t nibble1 = nibbles[0];
    uint8_t nibble2 = nibbles[1];
    uint8_t nibble3 = nibbles[2];
    uint8_t nibble4 = nibbles[3];

    switch(nibble1) {

        case 0x0:
            switch(nibble4) {
                case 0x0: clearScreen();
                break;
            }
        break;
        case 0x1: ;
        break;
    }

}

void chip8::clearScreen() {
    
    for(int i = 0; i < 32; i++) {
        for(int j = 0; j < 64; j++) {
            display[i][j] = 0;
        }
    }

}

void chip8::jump(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    uint16_t newAddress = (nibble1 << 8) | (nibble2 << 4) | nibble3;
    PC = newAddress;

}
