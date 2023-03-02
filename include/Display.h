#include <vector>
#include <SDL.h>
#include "Chip8.h"

using namespace std;

namespace Graphics {

    class Display {

        public:

            const int WIDTH = 640;
            const int HEIGHT = 480;
            SDL_Renderer *renderer;
            SDL_Window *window;
            SDL_Event event;
            vector<SDL_Point> pointsVector;

            void initGraphics();
            void quitGraphics();
            void createPointsVector(Chip8 chip);
            void drawVector(vector vector);

    };
}