//
// Created by 15355 on 2023/12/24.
//

#include "block.h"
#include <stdio.h>
#include "player.h"
#include "SDL2/SDL_image.h"
#include "utils/display.h"

enum blockKind {
   Grey=2,Basic=5,Cake=13,Life,Luck,Music,MagicCube,BlockKinds
};
enum magicBlock {
    /*magicCube,*/ MagicBlockKinds=1
};
enum direction {
    right,left
};

float BlockSize[BlockKinds];
struct block BlockHead;
static SDL_Texture *BlockTexture[BlockKinds-MagicBlockKinds];
static char *BlockTexturePath[]={
    "picture/blue.png","picture/green.png","picture/grey.png","picture/orange.png","picture/pink.png",
    "picture/basic.png","picture/basic1.png","picture/basic2.png","picture/basic3.png","picture/basic4.png",
    "picture/basic5.png","picture/basic6.png","picture/box.png","picture/cake.png","picture/life.png",
    "picture/luck.png","picture/music.png",
};
static SDL_Texture *ShadowTexture[2];
static char *ShadowTexturePath[2]={
        "picture/shadow_square.png","picture/shadow_cake.png",
};
SDL_Texture *magicCubeTexture[6];

void Block_Init(void) {
    for (int i = 0; i < BlockKinds - MagicBlockKinds; ++i) {
        BlockTexture[i] = IMG_LoadTexture(Renderer,BlockTexturePath[i]);
        if (BlockTexture[i] == NULL) {
            SDL_Log("Block_Init_Texture NO.%d failed: %s",i, SDL_GetError());
        }
        int tempW,tempH;
        SDL_QueryTexture(BlockTexture[i],NULL,NULL,&tempW,&tempH);
        BlockSize[i] = (float)tempW/2;
    }
    for (int i = 0; i < 2; ++i) {
        ShadowTexture[i] = IMG_LoadTexture(Renderer,ShadowTexturePath[i]);
        if (ShadowTexture[i] == NULL) {
            SDL_Log("Block_Init_Texture_Shadow failed: %s",SDL_GetError());
        }
    }
    IMG_Animation *magicCube = IMG_LoadAnimation("picture/magicCube.gif");
    for (int i = 0; i <= 5; ++i) {
        SDL_SetColorKey(*(magicCube->frames+i), SDL_TRUE, SDL_MapRGB((*(magicCube->frames+i))->format, 255, 255, 255));
        magicCubeTexture[i] = SDL_CreateTextureFromSurface(Renderer,*(magicCube->frames+i));
    }
    int tempW,tempH;
    SDL_QueryTexture(magicCubeTexture[0],NULL,NULL,&tempW,&tempH);
    BlockSize[MagicCube] = (float)tempW;
    IMG_FreeAnimation(magicCube);

    FILE *fp = NULL;
    fp = fopen("block.dat", "rb");
    if (fp == NULL) {
        printf("Save data failed: cannot open block.dat\n");
        return;
    }
    int count;
    fread(&count,sizeof count,1,fp);
    struct block *cur = &BlockHead;
    for (int i = 0; i < count; ++i) {
        struct block *new = malloc(sizeof *new);
        fread(new,sizeof *new,1,fp);
        if (new->kind < BlockKinds-MagicBlockKinds) {
            new->texture = BlockTexture[new->kind];
        } else if (new->kind == MagicCube) {
            new->texture = magicCubeTexture[1];
        }
        cur ->nextBlock = new;
        cur = new;
    }
    cur->nextBlock = NULL;
    fclose(fp);
}

void Block_Clear(void) {
    struct block *cur = BlockHead.nextBlock;
    while (cur != NULL) {
        struct block *now = cur;
        cur = cur->nextBlock;
        free(now);
    }
    BlockHead.nextBlock = NULL;
}

void Block_Create(float playerCenterX,float playerCenterY,float distance,int kind,int direction,float percent) {
    struct block *newBlock = malloc(sizeof *newBlock);
    newBlock->kind = kind;
    newBlock->isCheck = false;
    newBlock->nextBlock = BlockHead.nextBlock;
    BlockHead.nextBlock = newBlock;
    if (kind < BlockKinds-MagicBlockKinds) {
        newBlock->texture = BlockTexture[kind];
    } else if (kind == MagicCube) {
        newBlock->texture = magicCubeTexture[1];
    }
    int tempW,tempH;
    SDL_QueryTexture(newBlock->texture,NULL,NULL,&tempW,&tempH);
    newBlock->w = (float)tempW * percent;
    newBlock->h = (float)tempH * percent;
    if (direction == right) {
        newBlock->centerX = playerCenterX + distance;
    } else newBlock->centerX = playerCenterX - distance;
    newBlock->centerY = playerCenterY - distance * 2 / 3;
}

void Block_Draw(struct block *block,float percent) {
    if (block != NULL) {
        Display_DrawPicture(block->centerX - (block->w / 2)*percent, block->centerY - (block->w / 3)*percent,block->w*percent,block->h*percent, 0, NULL, block->texture);
    }
}

void Block_DrawShadow(struct block *block) {
    if (block != NULL) {
        SDL_Texture *shadow;
        if (block->kind == Cake) {
            shadow = ShadowTexture[1];
            Display_DrawPicture(block->centerX - (block->w / 2), block->centerY+block->h/4-block->w/ 3, block->w,
                                block->w * 2 / 3, 0, NULL, shadow);
        }
        else {
            shadow = ShadowTexture[0];
            Display_DrawPicture(block->centerX - (block->w / 2), block->centerY + (block->h - block->w), block->w,
                                block->w * 2 / 3, 0, NULL, shadow);
        }
    }
}

SDL_Texture *Block_QueryTexture(int kind,int index) {
    if (kind < BlockKinds- MagicBlockKinds) {
        return BlockTexture[kind];
    }
    if (kind == MagicCube) {
        if (index < 6) return magicCubeTexture[index];
        else return  NULL;
    }
}

void Block_Quit(void) {
    for (int i = 0; i < BlockKinds-MagicBlockKinds; ++i) {
        SDL_DestroyTexture(BlockTexture[i]);
    }
    for (int i = 0; i < 2; ++i) {
        SDL_DestroyTexture(ShadowTexture[i]);
    }
    for (int i = 0; i < 6; ++i) {
        SDL_DestroyTexture(magicCubeTexture[i]);
    }
    FILE *fp = NULL;
    fp = fopen("block.dat", "wb");
    if (fp == NULL) {
        printf("Save data failed: cannot open block.dat\n");
        return;
    }
    struct block *cur = BlockHead.nextBlock;
    int count = 0;
    while (cur != NULL) {
        count++;
        cur =cur->nextBlock;
    }
    fwrite(&count,sizeof count,1,fp);
    cur = BlockHead.nextBlock;
    while (cur != NULL) {
        fwrite(cur,sizeof *cur,1,fp);
        cur =cur->nextBlock;
    }
    fclose(fp);
}

void Block_Destroy(struct block * uselessBlock) {
    free(uselessBlock);
}