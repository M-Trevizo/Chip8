#include <iostream>
#include <fstream>
#include <stdint.h>
#include <array>
#include <Chip8.h>
#include <Display.h>

using namespace std;
using namespace CPU;

void Chip8::loadRom(string path) {

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

void Chip8::loadFont() {

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

uint16_t Chip8::fetch() {

    uint8_t highByte = mem[PC];
    PC++;
    uint8_t lowByte = mem[PC];
    PC++;
    uint16_t opCode = (highByte << 8) | lowByte;
    return opCode;

}

array<uint8_t, 4> Chip8::decode(uint16_t opCode) {

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

void Chip8::execute(array<uint8_t, 4> nibbles) {
    
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
        case 0x1: jump(nibble2, nibble3, nibble4);
        break;
        case 0x6: setRegister(nibble2, nibble3, nibble4);
        break;
        case 0x7: add(nibble2, nibble3, nibble4);
        break;
        case 0xA: setIndex(nibble2, nibble3, nibble4);
        break;
        case 0xD: draw(nibble2, nibble3, nibble4);
        break;
    }

}

// 00E0
void Chip8::clearScreen() {
    
    for(int i = 0; i < 32; i++) {
        for(int j = 0; j < 64; j++) {
            display.displayState[i][j] = 0;
        }
    }

}

// 1NNN
void Chip8::jump(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    uint16_t newAddress = (nibble1 << 8) | (nibble2 << 4) | nibble3;
    PC = newAddress;

}

// 6XNN
void Chip8::setRegister(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    uint8_t byte = (nibble2 << 4) | nibble3;
    varReg[nibble1] = byte;

}

// 7XNN
void Chip8::add(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    uint8_t byte = (nibble2 << 4) | nibble3;
    varReg[nibble1] += byte;

}

// ANNN
void Chip8::setIndex(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    uint16_t address = (nibble1 << 8) | (nibble2 << 4) | nibble3;
    I = address;

}

// DXYN
void Chip8::draw(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    // TODO: Find a way to make SDL draw this array;
   
    uint16_t bytePointer = I;
    uint8_t bytesLeft = nibble3;
    uint8_t YLocation = nibble2 % 32;

    while(bytesLeft > 0) {

        uint8_t byteToDraw = mem[bytePointer];
        uint8_t XLocation = nibble1 % 64;
        uint8_t mask = 0xF0;

        while(mask > 0) {
            uint8_t bitToEnter = mask & byteToDraw;
            display[YLocation][XLocation] ^= bitToEnter;
            mask >> 1;
            XLocation++;
        }

        YLocation++;
        bytePointer++;
        bytesLeft--;
    }

    display.createPointsVector();
    display.drawVector();
}