
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

    void onKeyPressed(SDL_Keycode keyCode);
    bool any_key_down() { return is_key_down;}
    void set_active_key(SDL_Keycode keycode, bool is_down);
    SDL_Keycode get_active_key() { return active_key;}

    ~Window() = default;

private:
    SDL_Event event;
    int width, height;
    //The window we'll be rendering to
    SDL_Window* window = NULL;
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    bool is_key_down = false;
    SDL_Keycode active_key = SDLK_UNKNOWN;

    void init_sdl();
    SDL_Window* init_window();
};


#endif //SOFTRENDERER_WINDOW_H
