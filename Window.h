
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

    bool is_mouse_init() { return mouse_init; }
    int get_mouse_x() { return mouse_x; }
    int get_mouse_y() { return mouse_y; }
    void on_mouse_move(int x, int y);
    void on_key_pressed(SDL_Keycode keyCode);
    bool any_key_down() const { return is_key_down;}
    bool is_left_mouse_down() { return left_mouse_down; }
    bool is_right_mouse_down() { return right_mouse_down; }
    void set_active_key(SDL_Keycode keycode, bool is_down);
    SDL_Keycode get_active_key() const { return active_key;}

    ~Window() = default;

private:
    SDL_Event event;
    int width, height;
    //The window we'll be rendering to
    SDL_Window* window = nullptr;
    //The surface contained by the window
    SDL_Surface* screenSurface = nullptr;

    void init_sdl();
    SDL_Window* init_window();

    bool mouse_init = false;
    int mouse_x = 0;
    int mouse_y = 0;
    bool is_key_down = false;
    bool left_mouse_down = false;
    bool right_mouse_down = false;
    SDL_Keycode active_key = SDLK_UNKNOWN;
};


#endif //SOFTRENDERER_WINDOW_H
