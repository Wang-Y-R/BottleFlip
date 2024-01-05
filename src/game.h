//
// Created by 15355 on 2023/12/24.
//

#ifndef BOTTLEFLIP_GAME_H
#define BOTTLEFLIP_GAME_H

#include "SDL2/SDL.h"
#include <stdbool.h>
bool Game(void);
static void Game_Init(void);
static void Game_Draw(void);
static void Game_DrawBackground(void);
static bool Game_BlockCreateAnimation(void);
static bool Game_PlayerBornAnimation(void);
static void Game_Quit(void);
#endif //BOTTLEFLIP_GAME_H
