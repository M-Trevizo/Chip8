#include <vector>
#include <SDL.h>

using namespace std;

namespace Graphics {

    class Display {

        public:

            static const int WIDTH = 640;
            static const int HEIGHT = 480;
            SDL_Renderer *renderer;
            SDL_Window *window;
            SDL_Event event;
            vector<SDL_Point> pointsVector;
            int displayState[32][64] = {};

            void initGraphics();
            void quitGraphics();
            void createPointsVector();
            void drawVector();

    };
}