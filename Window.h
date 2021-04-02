
#ifndef SOFTRENDERER_WINDOW_H
#define SOFTRENDERER_WINDOW_H

#include <SDL.h>
#include "Color.h"

class Window {
public:
    bool is_running = true;

    void init(int width, int height);
    void update(Color* color_buffer);
    void set_title(const char* title);
    void stop();

    ~Window() {}
private:
    SDL_Event event;
    int width, height;
    //The window we'll be rendering to
    SDL_Window* window = NULL;
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

private:
    void init_sdl();
    SDL_Window* init_window();
};


#endif //SOFTRENDERER_WINDOW_H
