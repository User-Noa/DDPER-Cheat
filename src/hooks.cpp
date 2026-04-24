#include "hooks.h"
#include "cheat.h"
#include <dlfcn.h>
#include <mutex>

typedef uint32_t (*SDL_GetRelativeMouseState_t)(int *x, int *y);
typedef int (*SDL_PollEvent_t)(SDL_Event* event);
typedef void (*SDL_GL_SwapWindow_t)(SDL_Window* window);

SDL_GetRelativeMouseState_t o_SDL_GetRelativeMouseState = nullptr;
SDL_PollEvent_t o_SDL_PollEvent = nullptr;

extern "C" uint32_t SDL_GetRelativeMouseState(int *x, int *y) {
    if (!o_SDL_GetRelativeMouseState) o_SDL_GetRelativeMouseState = (SDL_GetRelativeMouseState_t)dlsym(RTLD_NEXT, "SDL_GetRelativeMouseState");
    uint32_t state = o_SDL_GetRelativeMouseState(x, y);
    if (menuOpen) { if (x) *x = 0; if (y) *y = 0; return 0; }
    return state;
}

extern "C" int SDL_PollEvent(SDL_Event* event) {
    if (!o_SDL_PollEvent) o_SDL_PollEvent = (SDL_PollEvent_t)dlsym(RTLD_NEXT, "SDL_PollEvent");
    int result = o_SDL_PollEvent(event);
    if (result) {
        if (imguiInitDone) {
            std::lock_guard<std::mutex> lock(eventMutex);
            eventQueue.push_back(*event);
        }

        if (event->type == SDL_MOUSEMOTION && !menuOpen) { mouseX = event->motion.x; mouseY = event->motion.y; }
        if (event->type == SDL_MOUSEBUTTONDOWN) {
            if (event->button.button == SDL_BUTTON_LEFT) leftMouseDown = true;
            if (event->button.button == SDL_BUTTON_RIGHT) rightMouseDown = true;
        }
        if (event->type == SDL_MOUSEBUTTONUP) {
            if (event->button.button == SDL_BUTTON_LEFT) leftMouseDown = false;
            if (event->button.button == SDL_BUTTON_RIGHT) rightMouseDown = false;
        }
        if (waitingForKey > 0) {
            if (event->type == SDL_KEYDOWN) {
                if (event->key.keysym.sym != SDLK_ESCAPE) {
                    if (waitingForKey == 1) aimHotkey = event->key.keysym.sym;
                    if (waitingForKey == 2) espHotkey = event->key.keysym.sym;
                }
                waitingForKey = 0; return 0; 
            }
            if (event->type == SDL_MOUSEBUTTONDOWN) {
                if (waitingForKey == 1) aimHotkey = -event->button.button;
                if (waitingForKey == 2) espHotkey = -event->button.button;
                waitingForKey = 0; return 0;
            }
        } else {
            int currentKey = 0;
            if (event->type == SDL_KEYDOWN) currentKey = event->key.keysym.sym;
            else if (event->type == SDL_MOUSEBUTTONDOWN) currentKey = -event->button.button;
            if (currentKey != 0) {
                if (currentKey == SDLK_INSERT) {
                    menuOpen = !menuOpen;
                    if (menuOpen) { SDL_SetRelativeMouseMode(SDL_FALSE); SDL_StartTextInput(); } 
                    else { SDL_SetRelativeMouseMode(SDL_TRUE); SDL_StopTextInput(); }
                } 
                else if (currentKey == aimHotkey) aimbotEnabled = !aimbotEnabled;
                else if (currentKey == espHotkey) espEnabled = !espEnabled;
            }
        }
        if (menuOpen && (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || 
            event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEWHEEL ||
            event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)) return 0; 
    }
    return result;
}
