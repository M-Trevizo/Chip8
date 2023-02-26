#include <iostream>
#include <fstream>
#include <chip8.h>

using namespace std;
using namespace CPU;

int main(int argc, char** argv) {

    // Check arguments for single filepath
    if(argc > 2) {
        cout << "Too many arguments given." << endl;
        return 1;
    }

    string path = argv[1];
    
}