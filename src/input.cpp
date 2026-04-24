#include "input.h"
#include <SDL2/SDL.h>
#include <cstring>

void SimulateKeyClick(int sdlKey) {
    SDL_Event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = SDL_KEYDOWN;
    ev.key.keysym.sym = sdlKey;
    SDL_PushEvent(&ev);
    
    ev.type = SDL_KEYUP;
    SDL_PushEvent(&ev);
}

void SimulateMouseLeftClick() {
    SDL_Event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = SDL_MOUSEBUTTONDOWN;
    ev.button.button = SDL_BUTTON_LEFT;
    ev.button.state = SDL_PRESSED;
    SDL_PushEvent(&ev);
    
    ev.type = SDL_MOUSEBUTTONUP;
    ev.button.state = SDL_RELEASED;
    SDL_PushEvent(&ev);
}
