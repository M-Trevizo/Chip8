#include <iostream>
#include <fstream>
#include <stdint.h>
#include <array>
#include <bitset>
#include <windows.h>
#include "../include/Chip8.h"

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

        uint8_t byte = pBuf->sbumpc();
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

    uint16_t mask = 0xF;

    nibble4 = opCode & mask;
    opCode = opCode >> 4;
    nibble3 = opCode & mask;
    opCode = opCode >> 4;
    nibble2 = opCode & mask;
    opCode = opCode >> 4;
    nibble1 = opCode & mask;

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
                case 0xE: returnFromSubroutine();
                break;
            }
        break;
        case 0x1: jump(nibble2, nibble3, nibble4);
        break;
        case 0x2: callSubroutine(nibble2, nibble3, nibble4);
        break;
        case 0x3: skipIfEquals(nibble2, nibble3, nibble4);
        break;
        case 0x4: skipIfNotEquals(nibble2, nibble3, nibble4);
        break;
        case 0x5: skipXEqualsY(nibble2, nibble3);
        break;
        case 0x6: setRegister(nibble2, nibble3, nibble4);
        break;
        case 0x7: add(nibble2, nibble3, nibble4);
        break;
        case 0x8: {
            switch(nibble4) {
                case 0x0: setXtoY(nibble2, nibble3);
                break;
                case 0x1: setOr(nibble2, nibble3);
                break;
                case 0x2: setAnd(nibble2, nibble3);
                break;
                case 0x3: setXOR(nibble2, nibble3);
                break;
                case 0x4: setAdd(nibble2, nibble3);
                break;
                case 0x5: setXSubY(nibble2, nibble3);
                break;
                case 0x6: setRShift(nibble2, nibble3);
                break;
                case 0x7: setYSubX(nibble2, nibble3);
                break;
                case 0xE: setLShift(nibble2, nibble3);
                break;
            }
        }
        break;
        case 0x9: skipXNotEqualsY(nibble2, nibble3);
        break;
        case 0xA: setIndex(nibble2, nibble3, nibble4);
        break;
        case 0xB: jumpOffset(nibble2, nibble3, nibble4);
        break;
        case 0xC: getRandom(nibble2, nibble3, nibble4);
        break;
        case 0xD: draw(nibble2, nibble3, nibble4);
        break;
        case 0xE: {
            uint8_t byte = (nibble3 << 4) | nibble4;
            switch(byte) {
                case 0x9E: skipIfKeyPress(nibble2);
                break;
                case 0xA1: skipIfNoKeyPress(nibble2);
                break;
            }
        }
        break;
        case 0xF: {
            uint8_t byte = (nibble3 << 4) | nibble4;
            switch(byte) {
                case 0x07: loadDelayTimer(nibble2);
                break;
                case 0x0A: ; // TODO: Dont forget to write this instruction
                break;
                case 0x15: setDelayTimer(nibble2);
                break;
                case 0x18: setSoundTimer(nibble2);
                break;
                case 0x1E: addI(nibble2);
                break;
                case 0x29: loadSprite(nibble2);
                break;
                case 0x33: storeBCD(nibble2);
                break;
                case 0x55: storeRegisters(nibble2);
                break;
                case 0x65: readRegisters(nibble2);
            }
        }
        break;
    }

    // Decrement sound and delay timers
    if(DT > 0) {
        DT--;
    }
    
    if(ST > 0) {
        ST--;
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

// 00EE
void Chip8::returnFromSubroutine() {

    PC = stack.top();
    stack.pop();
    
}

// 1NNN
void Chip8::jump(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    uint16_t newAddress = (nibble1 << 8) | (nibble2 << 4) | nibble3;
    PC = newAddress;

}

// 2NNN
void Chip8::callSubroutine(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    uint16_t address = (nibble1 << 8) | (nibble2 << 4) | nibble3;
    stack.push(PC);
    PC = address;

}

// 3XNN
void Chip8::skipIfEquals(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    uint8_t byte = (nibble2 << 4) | nibble3;
    if(varReg[nibble1] == byte) {
        PC += 2;
    }

}

// 4XNN
void Chip8::skipIfNotEquals(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    uint8_t byte = (nibble2 << 4) | nibble3;
    if(varReg[nibble1] != byte) {
        PC += 2;
    }

}

// 5XY0
void Chip8::skipXEqualsY(uint8_t nibble1, uint8_t nibble2) {

    uint8_t X = varReg[nibble1];
    uint8_t Y = varReg[nibble2];

    if(X == Y) {
        PC += 2;
    }

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

// 8XY0
void Chip8::setXtoY(uint8_t nibble1, uint8_t nibble2) {
    
    uint8_t Y = varReg[nibble2];
    varReg[nibble1] = Y;

}

// 8XY1
void Chip8::setOr(uint8_t nibble1, uint8_t nibble2) {

    uint8_t value = varReg[nibble1] | varReg[nibble2];
    varReg[nibble1] = value;

}

// 8XY2
void Chip8::setAnd(uint8_t nibble1, uint8_t nibble2) {

    uint8_t value = varReg[nibble1] & varReg[nibble2];
    varReg[nibble1] = value;

}

// 8XY3
void Chip8::setXOR(uint8_t nibble1, uint8_t nibble2) {

    uint8_t value = varReg[nibble1] ^ varReg[nibble2];
    varReg[nibble1] = value;

}

// 8XY4
void Chip8::setAdd(uint8_t nibble1, uint8_t nibble2) {

    int value = varReg[nibble1] + varReg[nibble2];

    if(value > 0xFF) {
        uint8_t mask = 0xFF;
        value = value & mask;
        varReg[0xF] = 1;
    }
    else {
        varReg[0xF] = 0;
    }

    varReg[nibble1] = value;

}

// 8XY5
void Chip8::setXSubY(uint8_t nibble1, uint8_t nibble2) {
    
    uint8_t X = varReg[nibble1];
    uint8_t Y = varReg[nibble2];
    
    if(X > Y) {
        varReg[0xF] = 1;
    }
    else {
        varReg[0xF] = 0;
    }

    uint8_t value = X - Y;
    varReg[nibble1] = value;

}

// 8XY6
void Chip8::setRShift(uint8_t nibble1, uint8_t nibble2) {

    uint8_t X = varReg[nibble1];
    uint8_t mask = 0x1;

    if((X & mask) == 1) {
        varReg[0xF] = 1;
    }
    else {
        varReg[0xF] = 0;
    }

    varReg[nibble1] = X / 2;

}

// 8XY7
void Chip8::setYSubX(uint8_t nibble1, uint8_t nibble2) {

    uint8_t X = varReg[nibble1];
    uint8_t Y = varReg[nibble2];
    
    if(Y > X) {
        varReg[0xF] = 1;
    }
    else {
        varReg[0xF] = 0;
    }

    uint8_t value = Y - X;
    varReg[nibble1] = value;

}

// 8XYE
void Chip8::setLShift(uint8_t nibble1, uint8_t nibble2) {

    uint8_t X = varReg[nibble1];
    uint8_t mask = 0x80;

    if((X & mask) == 1) {
        varReg[0xF] = 1;
    }
    else {
        varReg[0xF] = 0;
    }

    varReg[nibble1] = X * 2;

}

// 9XY0
void Chip8::skipXNotEqualsY(uint8_t nibble1, uint8_t nibble2) {

    uint8_t X = varReg[nibble1];
    uint8_t Y = varReg[nibble2];

    if(X != Y) {
        PC += 2;
    }

}

// ANNN
void Chip8::setIndex(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    uint16_t address = (nibble1 << 8) | (nibble2 << 4) | nibble3;
    I = address;

}

// BNNN
void Chip8::jumpOffset(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    uint8_t offset = varReg[0x0];
    uint16_t address = (nibble1 << 8) | (nibble2 << 4) | nibble3;
    uint16_t addressToJump = address + offset;
    PC = addressToJump;

}

// CNNN
void Chip8::getRandom(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {

    uint8_t byte = (nibble2 << 4) | nibble3;
    int randomNum = rand() % 256;
    uint8_t randomByte = randomNum & byte;
    varReg[nibble1] = randomByte;

}

// DXYN
void Chip8::draw(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {
   
    uint16_t bytePointer = I;
    uint8_t bytesLeft = nibble3;
    uint8_t YLocation = varReg[nibble2] % 32;
    varReg[0xF] = 0;

    while(bytesLeft > 0) {

        bitset<8> bitsToDraw = mem[bytePointer];        // Index 0 of bitset is Rightmost bit
        uint8_t XLocation = varReg[nibble1] % 64;
        uint8_t mask = 0x1;

        for(int i = 7; i >= 0; i--) {

            if(display.displayState[YLocation][XLocation] == 1) {
                display.displayState[YLocation][XLocation] ^= bitsToDraw[i];
                varReg[0xF] = 1;
            }
            else {
                display.displayState[YLocation][XLocation] = bitsToDraw[i];
            }
            
            XLocation++;
        }
        
        YLocation++;
        bytePointer++;
        bytesLeft--;

    }

    display.createPointsVector();
    display.drawVector();
}

// EX9E
void Chip8::skipIfKeyPress(uint8_t nibble1) {

    int scanCode = keyMap[varReg[nibble1]];
    if(currentKeyStates[scanCode]) {
        PC += 2;
    }

}

// EXA1
void Chip8::skipIfNoKeyPress(uint8_t nibble1) {

    SDL_Scancode scanCode = keyMap[varReg[nibble1]];
    if(!currentKeyStates[scanCode]) {
        PC += 2;
    }

}

// FX07
void Chip8::loadDelayTimer(uint8_t nibble1) {

    varReg[nibble1] = DT;

}

// FX0A
void Chip8::getKeyPress(uint8_t nibble1) {

    bool waiting = true;
    while(waiting) {
        printf("Waiting for KeyPress");
        if(currentKeyStates[SDL_SCANCODE_1]) {
            varReg[nibble1] = 0x1;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_2]) {
            varReg[nibble1] = 0x2;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_3]) {
            varReg[nibble1] = 0x3;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_4]) {
            varReg[nibble1] = 0xC;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_Q]) {
            varReg[nibble1] = 0x4;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_W]) {
            varReg[nibble1] = 0x5;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_E]) {
            varReg[nibble1] = 0x6;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_R]) {
            varReg[nibble1] = 0xD;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_A]) {
            varReg[nibble1] = 0x7;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_S]) {
            varReg[nibble1] = 0x8;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_D]) {
            varReg[nibble1] = 0x9;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_F]) {
            varReg[nibble1] = 0xE;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_Z]) {
            varReg[nibble1] = 0xA;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_X]) {
            varReg[nibble1] = 0x0;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_C]) {
            varReg[nibble1] = 0xB;
            waiting = false;
        }
        else if(currentKeyStates[SDL_SCANCODE_V]) {
            varReg[nibble1] = 0xF;
            waiting = false;
        }
    }

}

// FX15
void Chip8::setDelayTimer(uint8_t nibble1){

    DT = varReg[nibble1];

}

// FX18
void Chip8::setSoundTimer(uint8_t nibble1) {

    ST = varReg[nibble1];

}

// FX1E
void Chip8::addI(uint8_t nibble1) {

    I += varReg[nibble1];

}

// FX29
void Chip8::loadSprite(uint8_t nibble1) {

    uint8_t sprite = varReg[nibble1];
    uint16_t startingLocation = 0x050;
    uint16_t spriteLocation = startingLocation + (sprite * 5);  // sprite value * 5 is calculated offset
    I = spriteLocation;

}

// FX33
void Chip8::storeBCD(uint8_t nibble1) {

    int value = varReg[nibble1];
    int hundreds = value / 100;
    int tens = (value % 100) / 10;
    int ones = value % 10;

    uint16_t copyOfI = I;
    mem[copyOfI] = hundreds;
    copyOfI++;
    mem[copyOfI] = tens;
    copyOfI++;
    mem[copyOfI] = ones;

}

// FX55
void Chip8::storeRegisters(uint8_t nibble1) {

    uint16_t copyOfI = I;
    for(int i = 0; i <= nibble1; i++) {
        mem[copyOfI] = varReg[i];
        copyOfI++;
    }

}

// FX65
void Chip8::readRegisters(uint8_t nibble1) {

    uint16_t copyOfI = I;
    for(int i = 0; i <= nibble1; i++) {
        varReg[i] = mem[copyOfI];
        copyOfI++;
    }

}