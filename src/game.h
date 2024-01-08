//
// Created by 15355 on 2023/12/24.
//

#ifndef BOTTLEFLIP_GAME_H
#define BOTTLEFLIP_GAME_H

#include "SDL2/SDL.h"
#include <stdbool.h>
#include "block.h"

#define GridSize 15
#define MaxStrength 500.0

bool Game(void);
static void Game_Init(void);

static void Game_DrawAll(void);
static void Game_DrawText();
static void Game_DrawBlockAndBackground(struct block *blockLink);
static void Game_DrawBackground(void);

static bool Game_BlockCreateAnimation(bool first);
static bool Game_PlayerBornAnimation(void);
static bool Game_TextAppearAnimation(void);
static void Game_AddScoreAnimation(int score);

static void Game_CreateNextBlock(bool first);

static bool Game_Jump(void);
static bool Game_GatherStrength(void);
static bool Game_JumpAnimation(float strength);

static int Game_Check(void);
static void Game_Scroll(void);
static void Game_BlockEventCheck(int kind);

static void Game_Quit(void);
#endif //BOTTLEFLIP_GAME_H
