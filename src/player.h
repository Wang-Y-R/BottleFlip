//
// Created by 15355 on 2023/12/24.
//

#ifndef BOTTLEFLIP_PLAYER_H
#define BOTTLEFLIP_PLAYER_H
#include "SDL2/SDL.h"
#include "stdbool.h"
extern struct player{ //玩家信息
    SDL_Texture *texture;
    float x;
    float y;
    float w;
    float h;
    float centerX;
    float centerY;
    double angle;
    int score;
    int combo;
    int statue;
    int color;
    int lives;
    int AI;
    int star;
    bool isCheat;
} Player;

void Player_Init();

void Player_Draw(float percent);

void Player_ChangeTexture(int kind);

static void Player_Clear();

void Player_Quit();

#endif //BOTTLEFLIP_PLAYER_H
