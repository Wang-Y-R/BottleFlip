//
// Created by 15355 on 2023/12/24.
//

#include "input.h"
static SDL_Event Event;//事件

void Input_Clear(void) {
    for (int i = 0; i < 512; ++i) {
        Keyboard[i]=0;
    }
    Mouse.status = false;
    Mouse.move = false;
    while (SDL_PollEvent(&Event));
}

bool Input_GetEvent(void) {
    while (SDL_PollEvent(&Event)) {
        Mouse.move = false;
        switch (Event.type) {
            case SDL_KEYUP:
                Keyboard[Event.key.keysym.scancode] = false;
                break;
            case SDL_KEYDOWN:
                Keyboard[Event.key.keysym.scancode] = true;
                break;
            case SDL_MOUSEBUTTONUP:
                Mouse.status = true;
                Mouse.x = Event.button.x;
                Mouse.y = Event.button.y;
                Mouse.clicks = Event.button.clicks;
                break;
            case SDL_MOUSEBUTTONDOWN:
                Mouse.status = false;
                Mouse.x = Event.button.x;
                Mouse.y = Event.button.y;
                Mouse.clicks = Event.button.clicks;
                break;
            case SDL_MOUSEMOTION:
                Mouse.move = true;
                Mouse.x = Event.button.x;
                Mouse.y = Event.button.y;
                break;
            case SDL_QUIT:
                return false;
            default:
                break;
        }
    }
    return true;
}