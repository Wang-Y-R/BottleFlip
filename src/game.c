//
// Created by 15355 on 2023/12/24.
//

#include "game.h"
#include "player.h" //玩家（棋子）相关
#include "block.h"  //方块相关
#include "utils/display.h"
#include "utils/input.h"
#include "utils/audio.h"

bool Game() {
    Game_Init();

    Block_Create(Player.centerX,Player.centerY,0,1,1);
    if (!Game_BlockCreateAnimation() || !Game_PlayerBornAnimation()) {
        Game_Quit();
        return false;
    }
    Game_Draw();
    Player_Draw();
    SDL_RenderPresent(Renderer);
    SDL_Delay(500);
    Game_Quit();
    return true;
}

static void Game_Draw() {
    Game_DrawBackground();
    struct block *cur = BlockHead.nextBlock;
    while (cur != NULL) {
        Block_Draw(cur);
        cur = cur->nextBlock;
    }
    SDL_RenderPresent(Renderer);
}

static void Game_DrawBackground(void) {
    SDL_SetRenderDrawColor(Renderer,Color.MistyRose.r,Color.MistyRose.g,Color.MistyRose.b,Color.MistyRose.a);
    SDL_RenderClear(Renderer);
}

static bool Game_BlockCreateAnimation(void) {
    float height = 250,accelerate = 1,speed = 0,centreYSave = BlockHead.nextBlock->centerY;
    BlockHead.nextBlock->centerY -= height;
    while (speed >= 2 || speed <= -2 || height >= 5) {
        if (!Input_GetEvent()) return false;
        Game_DrawBackground();
        struct block *cur = BlockHead.nextBlock->nextBlock;
        while (cur != NULL) {
            Block_Draw(cur);
            cur = cur->nextBlock;
        }
        float y = BlockHead.nextBlock->centerY;
        if (y - centreYSave > 0) y = centreYSave;
        BlockHead.nextBlock->centerY = centreYSave;
        Block_DrawShadow(BlockHead.nextBlock);
        BlockHead.nextBlock->centerY = y;
        Block_Draw(BlockHead.nextBlock);
        height -= speed;
        BlockHead.nextBlock->centerY += speed;
        speed += accelerate;
        if (height <= 0) speed = -speed/2;
        SDL_RenderPresent(Renderer);
        SDL_Delay(20);
    }
    BlockHead.nextBlock->centerY = centreYSave;
    return true;
}

static bool Game_PlayerBornAnimation(void){
    float height = 250,speed = 0,accelerate = 1,centreYSave = Player.centerY;
    Player.centerY -= height;
    while (speed >= 2 || speed <= -2 || height >= 5) {
        if (!Input_GetEvent()) return false;
        Game_DrawBackground();
        Player.centerY += speed;
        Block_Draw(BlockHead.nextBlock);
        Player_Draw();
        SDL_RenderPresent(Renderer);
        height -= speed;
        speed += accelerate;
        if (height <= 0) speed = -speed/2;
        SDL_Delay(20);
    }
    Player.centerY = centreYSave;
    return true;
}

static void Game_Init() {
    Block_Init();
    Player_Init();
}

static void Game_Quit() {
    Player_Quit();
    Block_Quit();
}