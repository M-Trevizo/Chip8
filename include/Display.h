#include <SDL.h>

namespace Graphics {

    class Display {

        public:

            const int WIDTH = 640;
            const int HEIGHT = 480;
            SDL_Renderer *renderer;
            SDL_Window *window;
            SDL_Event event;
            SDL_Rect rects[]

            void initGraphics();
            void quitGraphics();
            

    };
}