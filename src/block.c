//
// Created by 15355 on 2023/12/24.
//

#include "block.h"
#include "SDL2/SDL_image.h"
#include "utils/display.h"

enum blockKind {
    Blue,Green,Grey,Orange,Pink,Basic,BlockKinds
};
enum direction {
    right,left
};
struct block BlockHead;
static SDL_Texture *BlockTexture[BlockKinds];
static char *BlockTexturePath[]={
    "picture/blue.png","picture/green.png","picture/grey.png","picture/orange.png","picture/pink.png",
    "picture/basic.png",
};
static SDL_Texture *ShadowTexture[1];
static char *ShadowTexturePath[1]={
        "picture/shadow_210.png",
};


void Block_Init(void) {
    for (int i = 0; i < BlockKinds; ++i) {
        BlockTexture[i] = IMG_LoadTexture(Renderer,BlockTexturePath[i]);
        if (BlockTexture[i] == NULL) {
            SDL_Log("Block_Init_Texture NO.%d failed: %s",i, SDL_GetError());
        }
    }
    for (int i = 0; i < 1; ++i) {
        ShadowTexture[i] = IMG_LoadTexture(Renderer,ShadowTexturePath[i]);
        if (ShadowTexture[i] == NULL) {
            SDL_Log("Block_Init_Texture_Shadow failed: %s",SDL_GetError());
        }
    }
    BlockHead.nextBlock=NULL;
}

void Block_Create(float playerCenterX,float playerCenterY,float distance,int kind,int direction) {
    struct block *newBlock = malloc(sizeof *newBlock);
    newBlock->kind = kind;
    newBlock->nextBlock = BlockHead.nextBlock;
    BlockHead.nextBlock = newBlock;
    newBlock->texture = BlockTexture[kind];
    int tempW,tempH;
    SDL_QueryTexture(newBlock->texture,NULL,NULL,&tempW,&tempH);
    newBlock->w = (float)tempW;
    newBlock->h = (float)tempH;
    if (direction == right) {
        newBlock->centerX = playerCenterX + distance;
    } else newBlock->centerX = playerCenterX - distance;
    newBlock->centerY = playerCenterY - distance * 2 / 3;
}

void Block_Draw(struct block *block) {
    if (block != NULL) {
        Display_DrawPicture(block->centerX - block->w / 2, block->centerY - block->w / 3, 0, NULL, block->texture);
    }
}

void Block_DrawShadow(struct block *block) {
    SDL_Texture *shadow;
    if (block->w == 210.0) {
        shadow = ShadowTexture[0];
    }
    if (block != NULL) {
        Display_DrawPicture(block->centerX - block->w / 2, block->centerY + block->h - block->w, 0, NULL, shadow);
    }
}


void Block_Quit(void) {
    for (int i = 0; i < BlockKinds; ++i) {
        SDL_DestroyTexture(BlockTexture[i]);
    }
    for (int i = 0; i < 1; ++i) {
        SDL_DestroyTexture(ShadowTexture[i]);
    }
}

void Block_Destroy(struct block * uselessBlock) {
    free(uselessBlock);
}