#include <vector>
#include <SDL.h>
//#include <Chip8.h>

using namespace std;
using namespace CPU;

namespace Graphics {

    class Display {

        public:

            static const int WIDTH = 640;
            static const int HEIGHT = 480;
            SDL_Renderer *renderer;
            SDL_Window *window;
            SDL_Event event;
            vector<SDL_Point> pointsVector;
            static int displayState[32][64] = {};

            void initGraphics();
            void quitGraphics();
            void createPointsVector();
            void drawVector(vector vector);

    };
}