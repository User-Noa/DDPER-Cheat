#pragma once
#include <SDL2/SDL.h>

// اشاره‌گرهای اصلی SDL
extern uint32_t (*o_SDL_GetRelativeMouseState)(int* x, int* y);
extern int (*o_SDL_PollEvent)(SDL_Event* event);
extern void (*o_SDL_GL_SwapWindow)(SDL_Window* window);

// توابع جایگزین (export می‌شوند)
extern "C" {
    uint32_t SDL_GetRelativeMouseState(int* x, int* y);
    int SDL_PollEvent(SDL_Event* event);
    void SDL_GL_SwapWindow(SDL_Window* window);
}
