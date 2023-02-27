#include <SDL.h>

namespace Graphics {

    class Display {

        public:

            const int WIDTH = 512;
            const int HEIGHT = 256;
            SDL_Renderer *renderer;
            SDL_Window *window;
            SDL_Event event;

            void initGraphics();


    };
}