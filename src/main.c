//
// Created by 15355 on 2023/12/24.
//
#include "main.h"
#include "menu.h"
#include "utils/input.h"
#include "utils/display.h"
#undef main

SDL_Window *Window;//窗口
SDL_Surface *Surface;//表面
SDL_Renderer *Renderer;//渲染器
bool Keyboard[512];//记录键盘按键状态 0表示没有按下 1表示按下
struct mouse Mouse;

void Init();
void Quit();

int main(int argc,char *argv[]) {
    Init();
    Menu();
    Quit();
    return 0;
}

void Init() {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
        SDL_Log("SDL_InitSubSystem failed: %s", SDL_GetError());
    }
    if ((Window = SDL_CreateWindow(PROJECT_NAME, SDL_WINDOWPOS_CENTERED, 30,
                                   WINDOW_WIDTH, WINDOW_HEIGHT, SDL_FALSE)) == NULL) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
    }
    if (!(Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED))) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
    }
    if ((Surface = SDL_GetWindowSurface(Window)) == NULL) {
        SDL_Log("SDL_CreateSurface failed: %s", SDL_GetError());
    }
    if (TTF_Init() != 0) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
    }
    Input_Init();
}

void Quit() {
    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindowSurface(Window);
}