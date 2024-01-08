//
// Created by 15355 on 2023/12/24.
//

#ifndef BOTTLEFLIP_BLOCK_H
#define BOTTLEFLIP_BLOCK_H

#include "common.h"
extern struct block{
    SDL_Texture *texture;
    int kind;
    float w;
    float h;
    float centerX;
    float centerY;
    struct block* nextBlock;
}BlockHead;

void Block_Init(void);

void Block_Create(float playerCenterX,float playerCenterY,float distance,int kind,int direction,float percent);

void Block_Draw(struct block *block,float percent);

void Block_DrawShadow(struct block *block);

SDL_Texture *Block_QueryTexture(int kind,int index);

void Block_Quit(void);

void Block_Destroy(struct block * uselessBlock);

#endif //BOTTLEFLIP_BLOCK_H
