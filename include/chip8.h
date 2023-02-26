#include <array>
#include <stdint.h>
#include <stack>

using namespace std;

namespace CPU {

    class chip8 {

        public:
            array<uint8_t, 4096> mem;               // Memory 4kB RAM
            uint8_t PC;                             // Program Counter
            uint16_t I;                             // Index Register
            stack<uint16_t> stack;                  // Stack
            uint8_t DT;                             // Delay Timer
            uint8_t ST;                             // Sound Timer
            array<uint8_t, 16> varReg;              // Varibal Register; VF is flag register

            bool loadRom(string path);              // Load rom into memory;

    };
}