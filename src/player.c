//
// Created by 15355 on 2023/12/24.
//

#include "player.h"
#include <stdio.h>
#include "common.h"
#include "utils/display.h"
#include "SDL2/SDL_image.h"

struct player Player;


enum PlayerColor {
    /*Blue,Green,*/Grey=2,/*Orange,Pink,*/PlayerColorKinds=5
};
enum PlayerStatue {
    Normal,/*AI*/
};

static SDL_Texture *PlayerTexture[PlayerColorKinds];
static char *PlayerTexturePath[PlayerColorKinds] = {
        "picture/player-blue.png",
        "picture/player-green.png",
        "picture/player-grey.png",
        "picture/player-orange.png",
        "picture/player-pink.png",
};

void Player_Init() {
    for (int i = 0; i < PlayerColorKinds; ++i) {
        PlayerTexture[i] = IMG_LoadTexture(Renderer,PlayerTexturePath[i]);
        if (PlayerTexture[i] == NULL) {
            SDL_Log("Player_Init_Texture failed : %s",SDL_GetError());
        }
    }
    FILE *fp = NULL;
    fp = fopen("player.dat", "rb");
    if (fp == NULL) {
        printf("Save data failed: cannot open player.dat\n");
        Player_Clear();
        return;
    }
    fread(&Player,sizeof Player,1,fp);
    Player.texture = PlayerTexture[Player.color];
    fclose(fp);
}

void Player_Clear() {
    int tempW,tempH;
    SDL_QueryTexture(Player.texture,NULL,NULL,&tempW,&tempH);
    Player.w = (float)tempW;
    Player.h = (float)tempH;
    Player.angle = 0;
    Player.statue = Normal;
    Player.combo = Player.score = 0;
    Player.color = Grey;
    Player.centerX = WINDOW_WIDTH/2;
    Player.centerY = 650;
    Player.lives = 2;
    Player.AI = 1;
    Player.star =1;
    Player.isCheat = false;
}

void Player_Draw(float percent) {
    Player.x = Player.centerX - (Player.w / 2)*percent;
    Player.y = Player.centerY - (Player.h)*percent;
    Display_DrawPicture(Player.x,Player.y,Player.w*percent,Player.h*percent,Player.angle,NULL,Player.texture);
}

void Player_ChangeTexture(int kind) {
    Player.texture = PlayerTexture[kind];
    Player.color = kind;
}

void Player_Quit() {
    for (int i = 0; i < PlayerColorKinds; ++i) {
        SDL_DestroyTexture(PlayerTexture[i]);
    }
    FILE *fp = NULL;
    fp = fopen("player.dat", "wb");
    if (fp == NULL) {
        printf("Save data failed: cannot open player.dat\n");
        return;
    }
    fwrite(&Player,sizeof Player,1,fp);
    fclose(fp);
}