//
// Created by 15355 on 2023/12/24.
//
#include "main.h"
#include "menu.h"
#include "game.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"
#include "utils/input.h"
#include "utils/display.h"
#undef main

SDL_Window *Window;//窗口
SDL_Surface *Surface;//表面
SDL_Renderer *Renderer;//渲染器
bool Keyboard[512];//记录键盘按键状态 0表示没有按下 1表示按下
struct mouse Mouse;
bool isQuit;
struct color Color = {
       {255, 228, 225,255},{245, 255, 250,255},
         {119 , 136, 153, 255},{255, 248, 220,255},
       {255,255,255,255},{255,0,0,255},
};
int CurrentColor;
SDL_Color BackgroundColor[BackgroundColorKind] = {
        {255, 228, 225,255},{255, 204, 255,255},{204,229,255,255},
        {229,255,204,255},
};

Mix_Chunk *Music_buttonSelect;
Mix_Chunk *Music_createBlock;
Mix_Chunk *Music_heart;
Mix_Chunk *Music_jumping;
Mix_Chunk *Music_rising;
Mix_Chunk *Music_start;
Mix_Chunk *Music_pop;
Mix_Chunk *Music_magicCube;
Mix_Chunk *Music_changeColor;
Mix_Chunk *Music_coin;
Mix_Chunk *Music_robot;
Mix_Chunk *Music_star;
Mix_Chunk *Music_failed;
Mix_Chunk *Music_music1;
Mix_Chunk *Music_music2;
Mix_Chunk *Music_music3;

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
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) == -1)
        SDL_Log("%s", SDL_GetError());
    Music_buttonSelect = Mix_LoadWAV("music/buttonSelect.wav");
    Music_createBlock = Mix_LoadWAV("music/createBlock.wav");
    Music_heart = Mix_LoadWAV("music/heart.wav");
    Music_jumping = Mix_LoadWAV("music/jumping.wav");
    Music_rising = Mix_LoadWAV("music/rising4.wav");
    Music_start = Mix_LoadWAV("music/start.wav");
    Music_pop = Mix_LoadWAV("music/pop.wav");
    Music_magicCube = Mix_LoadWAV("music/magicCube.wav");
    Music_changeColor = Mix_LoadWAV("music/changeColor.wav");
    Music_coin = Mix_LoadWAV("music/coin.wav");
    Music_robot = Mix_LoadWAV("music/robot.wav");
    Music_star = Mix_LoadWAV("music/star.wav");
    Music_failed = Mix_LoadWAV("music/failed.wav");
    Music_music1 = Mix_LoadWAV("music/music1.wav");
    Music_music2 = Mix_LoadWAV("music/music2.wav");
    Music_music3 = Mix_LoadWAV("music/music3.wav");
    CurrentColor = 0;
    isQuit = false;
}

void Quit() {
    Mix_FreeChunk(Music_buttonSelect);
    Mix_FreeChunk(Music_createBlock);
    Mix_FreeChunk(Music_heart);
    Mix_FreeChunk(Music_jumping);
    Mix_FreeChunk(Music_rising);
    Mix_FreeChunk(Music_start);
    Mix_FreeChunk(Music_magicCube);
    Mix_FreeChunk(Music_changeColor);
    Mix_FreeChunk(Music_coin);
    Mix_FreeChunk(Music_star);
    Mix_FreeChunk(Music_robot);
    Mix_FreeChunk(Music_failed);
    Mix_FreeChunk(Music_music1);
    Mix_FreeChunk(Music_music2);
    Mix_FreeChunk(Music_music3);
    Mix_Quit();
    IMG_Quit();
    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindowSurface(Window);
}