#include <array>
#include <stdint.h>
#include <stack>
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
            void setIndex(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);               // ANNN
            void draw(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);                   // DXYN
    };
}