#include <array>
#include <stdint.h>
#include <stack>
#include <map>
#include "./Display.h"

using namespace std;
using namespace Graphics;

namespace CPU {

    class Chip8 {

        public:
            array<uint8_t, 4096> mem = {};              // Memory 4kB RAM
            uint16_t PC = 0x200;                        // Program Counter
            uint16_t I = 0;                             // Index Register
            stack<uint16_t> stack;                      // Stack
            uint8_t DT = 0;                             // Delay Timer
            uint8_t ST = 0;                             // Sound Timer
            array<uint8_t, 16> varReg = {};             // Varibal Register; VF is flag register
            Graphics::Display display;                  // Display instance
            bool romLoaded = false;                     // Is rom loaded into mem
            bool isRunning = false;                     // Boolean for game loop
            map<int, char> keyMap {                     // Keymap for hex value to key
                {0x1, '1'},
                {0x2, '2'},
                {0x3, '3'},
                {0xC, '4'},
                {0x4, 'Q'},
                {0x5, 'W'},
                {0x6, 'E'},
                {0xD, 'R'},
                {0x7, 'A'},
                {0x8, 'S'},
                {0x9, 'D'},
                {0xE, 'F'},
                {0xA, 'Z'},
                {0x0, 'X'},
                {0xB, 'C'},
                {0xF, 'V'}
            };
            void loadRom(string path);                  // Load rom into memory
            void loadFont();                            // Load font into memory
            uint16_t fetch();                           // Fetch Opcode
            array<uint8_t, 4> decode(uint16_t opCode);  // Decode Opcode
            void execute(array<uint8_t, 4> nibbles);    // Function to call execution function

            void clearScreen();                                                             // 00E0
            void returnFromSubroutine();                                                    // 00EE
            void jump(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);                   // 1NNN
            void callSubroutine(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);         // 2NNN
            void skipIfEquals(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);           // 3XNN
            void skipIfNotEquals(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);        // 4XNN
            void skipXEqualsY(uint8_t nibble1, uint8_t nibble2);                            // 5XY0
            void setRegister(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);            // 6XNN
            void add(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);                    // 7XNN
            void setXtoY(uint8_t nibble1, uint8_t nibble2);                                 // 8XY0
            void setOr(uint8_t nibble1, uint8_t nibble2);                                   // 8XY1
            void setAnd(uint8_t nibble1, uint8_t nibble2);                                  // 8XY2
            void setXOR(uint8_t nibble1, uint8_t nibble2);                                  // 8XY3
            void setAdd(uint8_t nibble1, uint8_t nibble2);                                  // 8XY4
            void setXSubY(uint8_t nibble1, uint8_t nibble2);                                // 8XY5
            void setRShift(uint8_t nibble1, uint8_t nibble2);                               // 8XY6
            void setYSubX(uint8_t nibble1, uint8_t nibble2);                                // 8XY7
            void setLShift(uint8_t nibble1, uint8_t nibble2);                               // 8XYE
            void skipXNotEqualsY(uint8_t nibble1, uint8_t nibble2);                         // 9XY0
            void setIndex(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);               // ANNN
            void jumpOffset(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);             // BNNN
            void getRandom(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);              // CNNN
            void draw(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);                   // DXYN
            void skipIfKeyPress(uint8_t nibble1);                                           // EX9E
            void skipIfNoKeyPress(uint8_t nibble1);                                         // EXA1
            void loadDelayTimer(uint8_t nibble1);                                           // FX07
            void getKeyPress(uint8_t nibble1);                                              // FX0A
            void setDelayTimer(uint8_t nibble1);                                            // FX15
            void setSoundTimer(uint8_t nibble1);                                            // FX18
            void addI(uint8_t nibble1);                                                     // FX1E
            void loadSprite(uint8_t nibble1);                                               // FX29
            void storeBCD(uint8_t nibble1);                                                 // FX33
            void storeRegisters(uint8_t nibble1);                                           // FX55
            void readRegisters(uint8_t nibble1);                                            // FX65

    };
}