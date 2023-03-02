#include <array>
#include <stdint.h>
#include <stack>

using namespace std;

namespace CPU {

    class Chip8 {

        public:
            array<uint8_t, 4096> mem = {};              // Memory 4kB RAM
            uint8_t PC = 0x200;                         // Program Counter
            uint16_t I = 0;                             // Index Register
            stack<uint16_t> stack = {};                 // Stack
            uint8_t DT = 0;                             // Delay Timer
            uint8_t ST = 0;                             // Sound Timer
            array<uint8_t, 16> varReg = {};             // Varibal Register; VF is flag register
            int display[32][64] = {};                   // Display grid
            bool romLoaded = false;                     // Is rom loaded into mem

            void loadRom(string path);                  // Load rom into memory
            void loadFont();                            // Load font into memory
            uint16_t fetch();                           // Fetch Opcode
            array<uint8_t, 4> decode(uint16_t opCode);  // Decode Opcode
            void execute(array<uint8_t, 4> nibbles);    // Function to call execution function

            void clearScreen();                                                             // 00E0
            void jump(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);                   // 1NNN
            void setRegister(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);            // 6XNN
            void add(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);                    // 7XNN 
            void setIndex(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);               // ANNN
            void draw(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);                   // DXYN
    };
}