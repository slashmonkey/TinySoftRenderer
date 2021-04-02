
#include "Window.h"
#include "Color.h"

void Window::init_sdl() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("[Error] SDL init : %s \n", SDL_GetError());
    } else {
        printf("SDL INITIALISED\n");
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        printf("Display mode is %dx%dpx @ %dhz\n", dm.w, dm.h, dm.refresh_rate);
    }
}

SDL_Window* Window::init_window() {
    SDL_Window* window = SDL_CreateWindow( "SoftRender", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
    if( window != NULL )
    {
        return window;
    }else{
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        exit(0);
    }
}

void Window::init(int width, int height) {
    this->width = width;
    this->height = height;
    init_sdl();
    window = init_window();
    //Get window surface
    screenSurface = SDL_GetWindowSurface(window);
}

void Window::update(Color* color_buffer) {
    SDL_LockSurface(screenSurface);
    Uint32* destPixels = (Uint32*)screenSurface->pixels;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            Color color = color_buffer[(height-1-y)*width + x];;
            Uint32 sdl_color = SDL_MapRGB( screenSurface->format, color.R(), color.G(), color.B());
            destPixels[x + y * width] = sdl_color;
        }
    }
    SDL_UnlockSurface(screenSurface);
    //Update the surface
    SDL_UpdateWindowSurface( window );

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            is_running = SDL_FALSE;
        }else if (event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    is_running = SDL_FALSE;
                    break;
                default:is_running = SDL_TRUE;
            }
        }
    }
}

void Window::stop() {
    SDL_DestroyWindow(window);
    window = nullptr;
    screenSurface = nullptr;
    SDL_Quit();
}

void Window::set_title(const char* title) {
    SDL_SetWindowTitle(window, title);
}
