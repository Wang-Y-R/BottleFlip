//
// Created by 15355 on 2023/12/24.
//

#include "game.h"
#include "player.h" //玩家（棋子）相关
#include "block.h"  //方块相关
#include "pause.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include "utils/display.h"
#include "utils/input.h"

enum blockKind {
    Blue,Green,Grey,Orange,Pink,Basic,Basic1,Basic2,Basic3,Basic4,Basic5,Basic6,Box,Cake,Life,Luck,Music,MagicCube,BlockKinds
};
enum magicBlock {
    magicCube, MagicBlockKinds
};
enum direction {
    right, left
} Direction;

static TTF_Font *ScoreFont, *ComboFont,*CheatFont;
static int Distance, Kind;
static SDL_Texture *StrengthOuter; //血条外壳
extern float BlockSize[BLOCK_KINDS];
extern Mix_Chunk *Music_createBlock;
extern Mix_Chunk *Music_heart;
extern Mix_Chunk *Music_jumping;
extern Mix_Chunk *Music_rising;
extern Mix_Chunk *Music_pop;
extern Mix_Chunk *Music_magicCube;
extern Mix_Chunk *Music_changeColor;
extern Mix_Chunk *Music_buttonSelect;
extern Mix_Chunk *Music_coin;
extern Mix_Chunk *Music_robot;
extern Mix_Chunk *Music_star;
extern Mix_Chunk *Music_start;
extern Mix_Chunk *Music_failed;
extern Mix_Chunk *Music_music1;
extern Mix_Chunk *Music_music2;
extern Mix_Chunk *Music_music3;

static SDL_Texture *heart,*starIn,*starOut,*star,*robot,*robotIn,*robotOut,*coin,*gearOut,*gearIn,*gear,*loveMusic;//图片
static SDL_FRect heartFRect = {WINDOW_WIDTH/2+WINDOW_WIDTH/8, 40, 70, 70},gearFRect = {WINDOW_WIDTH - 110, 10, 100, 100},//对应的位置
          robotFRect = {WINDOW_WIDTH-110,120,70,70},starFRect = {WINDOW_WIDTH-110,200,70,70};

uint64_t checkTime;

bool Game() {
    //初始化
    //初始化
    Game_Init();
    //加载初始的方块和动画
    Game_CreateNextBlock(true);
    if (!Game_BlockCreateAnimation(true) || !Game_PlayerBornAnimation()) {
        Game_Quit();
        return false;
    }
    SDL_Delay(100);
    Game_CreateNextBlock(false);
    if (!Game_BlockCreateAnimation(false)) {
        Game_Quit();
        return false;
    }
    //开始游戏
    while (true) {
        uint64_t start = SDL_GetTicks64();//计时开始
        if (!Input_GetEvent()) {
            Game_Quit();
            return false;
        }
        if (Mouse.move) { //移动鼠标，检测是不是使用道具或者暂停
            SDL_FPoint fPoint= {Mouse.x,Mouse.y};
            if (SDL_PointInFRect(&fPoint,&gearFRect)) {
                if (gear != gearIn) {
                    gear = gearIn;
                    Mix_PlayChannel(-1,Music_buttonSelect,0);
                }
            } else if (SDL_PointInFRect(&fPoint,&robotFRect)) {
                if (robot != robotIn) {
                    robot = robotIn;
                    Mix_PlayChannel(-1,Music_buttonSelect,0);
                }
            } else if (SDL_PointInFRect(&fPoint,&starFRect)) {
                if (star != starIn) {
                    star = starIn;
                    Mix_PlayChannel(-1,Music_buttonSelect,0);
                }
            }
            else {
                gear = gearOut;
                robot = robotOut;
                star = starOut;
            }
        }
        if (Mouse.status) {
            SDL_FPoint fPoint= {Mouse.x,Mouse.y};
            if (SDL_PointInFRect(&fPoint,&gearFRect)) {
                gear = gearOut;
                if (!Pause()) {
                    if (isQuit) {
                        SDL_Log("!");
                        Game_Quit();
                        return false;
                    }else return true;
                }
            } else if (SDL_PointInFRect(&fPoint,&robotFRect) && Player.AI > 0) {
                Player.statue = true;
                Player.AI--;
                if (!Game_AIMode()){
                    Game_Quit();
                    return false;
                }
                Player.statue = false;
            } else if (SDL_PointInFRect(&fPoint,&starFRect) && Player.star > 0) {
                Player.star--;
                struct block *temp = BlockHead.nextBlock;
                BlockHead.nextBlock = temp->nextBlock;
                Block_Destroy(temp);
                Game_CreateNextBlock(false);
                if (!Game_BlockCreateAnimation(false)) {
                    Game_Quit();
                    return false;
                }
            }
        } else if (Keyboard[SDL_SCANCODE_SPACE] == true) {//蓄力开跳
            if (!Game_Jump()) {
                Game_Quit();
                return false;
            }
            switch (Game_Check()) {
                case 0://还在原来的方块,啥也不用干
                    break;
                case 1://跳到下一个方块了！先生成下一个方块,再根据玩家有没有继续跳检测要不要执行当前方块的事件
                    Game_Scroll();
                    Game_CreateNextBlock(false);
                    if (!Game_BlockCreateAnimation(false)) {
                        Game_Quit();
                        return false;
                    }
                    break;
                case -1://没有跳到下一个方块，返回主菜单
                    SDL_Log("!");
                    Game_Quit();
                    return true;
            }
        } else if (Keyboard[SDL_SCANCODE_ESCAPE] == true) {//ESC暂停
            if (!Pause()) {
                if (isQuit) {
                    Game_Quit();
                    return false;
                }else return true;
            }
            Game_DrawAll();
            SDL_RenderPresent(Renderer);
            Input_Clear();
        } else if (Player.statue == true) { //是AImode
            if (!Game_AIMode()) {
                Game_Quit();
                return false;
            }
        } else { //不是AI模式，也没入任何输入
            Game_BlockEventCheck();
            Game_DrawAll();
            SDL_RenderPresent(Renderer);
            uint64_t end = SDL_GetTicks64();                                //计时终点
            SDL_Delay((end - start) < (1000 / Fps) ? 1000 / Fps : 0);
        }
    }
}

static bool Game_AIMode(void) {
    uint64_t aiStart = SDL_GetTicks64();
    uint64_t aiEnd = SDL_GetTicks64();
    while (aiEnd - aiStart <= 2000) {
        if (!Input_GetEvent()) {
            return false;
        }
        if (Keyboard[SDL_SCANCODE_Q] == true) {
            Player.statue = false;
            Game_DrawAll();
            SDL_RenderPresent(Renderer);
            Input_Clear();
            break;
        }
        Game_DrawAll();
        SDL_RenderPresent(Renderer);
        aiEnd = SDL_GetTicks64();
    }
    if (Player.statue == true) {
        Game_BlockEventCheck();
        if (!Game_Jump()) {
            return false;
        }
        Game_Check();
        Game_Scroll();
        Game_CreateNextBlock(false);
        if (!Game_BlockCreateAnimation(false)) {
            return false;
        }
    }
    return true;
}


static void Game_DrawAll() {
    Game_DrawBlockAndBackground(BlockHead.nextBlock);
    Player_Draw(1);
    Game_DrawText();
}

static void Game_DrawText(void) {
    char Score[20] = {"Score:"}, Combo[10] = {"Combo:"}, number[10] = {'\0'};
    strcat(Score, itoa(Player.score, number, 10));
    strcat(Combo, itoa(Player.combo, number, 10));
    Display_DrawText(0, 0, Score, 0, 0, &Color.White, ScoreFont);
    Display_DrawText(0, 70, Combo, 0, 0, &Color.White, ComboFont);
    if (Player.statue == true) {
        char *ai = "AI Mode,press Q to quit";
        Display_DrawTextByCentre(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100, ai, &Color.White, ScoreFont);
    }
}

static void Game_DrawBackground(void) {
    SDL_SetRenderDrawColor(Renderer, BackgroundColor[CurrentColor].r, BackgroundColor[CurrentColor].g,
                           BackgroundColor[CurrentColor].b, BackgroundColor[CurrentColor].a);
    SDL_RenderClear(Renderer);
    Display_DrawPicture(gearFRect.x,gearFRect.y,gearFRect.w,gearFRect.h,0,NULL,gear);
    for (int i = 0; i < Player.lives; ++i) {
        Display_DrawPicture(heartFRect.x + (float)i * 80, heartFRect.y, heartFRect.w, heartFRect.h, 0, NULL, heart);
    }
    char robotText[5]={"x\0"},number[4]={"\0"},starText[5]={"x\0"};
    Display_DrawPicture(robotFRect.x,robotFRect.y,robotFRect.w,robotFRect.h,0,NULL,robot);
    strcat(robotText, itoa(Player.AI, number, 10));
    Display_DrawText(robotFRect.x+robotFRect.w,robotFRect.y+robotFRect.h/2, robotText, 40, 40, &Color.White, ScoreFont);
    Display_DrawPicture(starFRect.x,starFRect.y,starFRect.w,starFRect.h,0,NULL,star);
    strcat(starText, itoa(Player.star, number, 10));
    Display_DrawText(starFRect.x+starFRect.w,starFRect.y+starFRect.h/2,starText,40,40,&Color.White,ScoreFont);
    //是否作弊了
    if (Player.isCheat) {
        char *cheat = {"* The score will not be recorded *\0"};
        Display_DrawTextByCentre(WINDOW_WIDTH/2,175,cheat,&Color.Red,CheatFont);
    }
}

static void Game_DrawBlockAndBackground(struct block *blockLink) {
    struct block *cur = blockLink;
    Game_DrawBackground();
    while (cur != NULL) {
        Block_Draw(cur, 1);
        cur = cur->nextBlock;
    }
}

static bool Game_BlockCreateAnimation(bool first) {
    Input_Clear();
    float height = 250, accelerate = 1, speed = 0, centreYSave = BlockHead.nextBlock->centerY;
    BlockHead.nextBlock->centerY -= height;
    int play = 1;
    while (speed >= 2 || speed <= -2 || height >= 5) {
        Game_DrawBlockAndBackground(BlockHead.nextBlock->nextBlock);
        Game_DrawText();
        float y = BlockHead.nextBlock->centerY;
        if (y - centreYSave > 0) {
            y = centreYSave;
            if (play) {
                Mix_PlayChannel(7, Music_createBlock, 0);
                play = 0;
            }
        }
        BlockHead.nextBlock->centerY = centreYSave;
        Block_DrawShadow(BlockHead.nextBlock);
        BlockHead.nextBlock->centerY = y;
        Block_Draw(BlockHead.nextBlock, 1);
        height -= speed;
        BlockHead.nextBlock->centerY += speed;
        speed += accelerate;
        if (height <= 0) speed = -speed / 2;
        if (!first) {
            Player_Draw(1);
            Game_DrawText();
        }
        SDL_RenderPresent(Renderer);
        SDL_Delay(20);
        if (!Input_GetEvent()) return false;
        if (Mouse.status == true) break;
    }
    BlockHead.nextBlock->centerY = centreYSave;
    Game_DrawAll();
    SDL_RenderPresent(Renderer);
    checkTime = SDL_GetTicks64();
    return true;
}

static bool Game_PlayerBornAnimation(void) {
    Input_Clear();
    float height = 250, speed = 0, accelerate = 1, centreYSave = Player.centerY;
    Player.centerY -= height;
    while (speed >= 2 || speed <= -2 || height >= 5) {
        if (!Input_GetEvent()) return false;
        Game_DrawBlockAndBackground(BlockHead.nextBlock);
        Game_DrawText();
        Player.centerY += speed;
        Player_Draw(1);
        SDL_RenderPresent(Renderer);
        height -= speed;
        speed += accelerate;
        if (height <= 0) {
            Mix_PlayChannel(-1, Music_pop, 0);
            speed = -speed / 2;
        }
        SDL_Delay(20);
        if (Mouse.status == 1) break;
    }
    Player.centerY = centreYSave;
    return true;
}

static void Game_CreateNextBlock(bool first) {
    if (first) {
        Block_Create(Player.centerX, Player.centerY, 0, Basic, 1, 1);
        return;
    }
    float percent;
    do {
        Kind = Life;//rand() % BLOCK_KINDS;
        Direction = rand() % 2;
        Distance = rand() % (int) MaxStrength;
        Distance = Distance - Distance % GridSize;
        percent = (float) (120 - (float) (rand() % 30) - Player.score / 20.0) / 100;
        if (percent <= 0.5f) percent = 0.5f;
    } while ((float) Distance < BlockHead.nextBlock->w * percent ||
             (Direction == left && ((float) Distance + BlockSize[Kind] * percent) >= Player.centerX) ||
             (Direction == right && ((float) Distance + Player.centerX + BlockSize[Kind] * percent) > WINDOW_WIDTH));
    SDL_Log("kind = %d", Kind);
    Block_Create(Player.centerX, Player.centerY, (float) Distance, Kind, Direction, percent);
}

static bool Game_Jump(void) {
    if (!Game_GatherStrength()) {
        return false;
    }
    return true;
}

static bool Game_GatherStrength(void) {
    float strength = 0, speed = 5, playerXDistance =
            Player.centerX - BlockHead.nextBlock->nextBlock->centerX, playerYDistance =
            Player.centerY - BlockHead.nextBlock->nextBlock->centerY;
    Mix_PlayChannel(6, Music_rising, 0);
    while (Keyboard[SDL_SCANCODE_SPACE] == true || Player.statue == true) {
        uint64_t start = SDL_GetTicks64();
        if (!Input_GetEvent()) {
            return false;
        }
        strength += speed;
        if (strength == MaxStrength) speed = -speed;
        if (strength == 0) {
            speed = -speed;
            Mix_PlayChannel(6, Music_rising, 0);
        }
        if (strength == (float) Distance && Player.statue == true) {
            break;
        }
        //人物以及方块动画
        Game_DrawBlockAndBackground(BlockHead.nextBlock->nextBlock->nextBlock);
        Block_Draw(BlockHead.nextBlock, 1);
        float percent = (float) ((5 * MaxStrength - strength) / (MaxStrength * 5));
        Player.centerX = BlockHead.nextBlock->nextBlock->centerX + playerXDistance * percent;
        Player.centerY = BlockHead.nextBlock->nextBlock->centerY + playerYDistance * percent;
        Block_Draw(BlockHead.nextBlock->nextBlock, percent);
        Player_Draw(percent);
        Game_DrawText();
        //蓄力条动画
        Display_DrawPictureByCentre(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 30, 0, 0, 0, NULL, StrengthOuter);
        SDL_FRect strengthFRect = {WINDOW_WIDTH / 2 - MaxStrength / 2, WINDOW_HEIGHT - 50, strength, 40};
        SDL_Color color = {(int) (strength / MaxStrength * 255),
                           (int) (((MaxStrength - strength) * 1.0 / MaxStrength) * 255), 0, 255};
        Display_FillFRect(&strengthFRect, &color);
        SDL_RenderPresent(Renderer);
        uint64_t end = SDL_GetTicks64();
        uint64_t pause = end - start;
        SDL_Delay(pause < 10 ? 10 - pause : 0);
    }
    Mix_Pause(6);
    //恢复数据
    Player.centerX = playerXDistance + BlockHead.nextBlock->nextBlock->centerX;
    Player.centerY = playerYDistance + BlockHead.nextBlock->nextBlock->centerY;
    if (!Game_JumpAnimation(strength)) {
        return false;
    }
    return true;
}

static bool Game_JumpAnimation(float strength) {
    Mix_PlayChannel(-1, Music_jumping, 0);
    float Accelerate = 0.2f, SpeedX = (strength / 90), SpeedY = Accelerate * 90 / 2,
            SavePlayerCenterY = Player.centerY, Start = Player.centerX, StartY = Player.centerY;
    float direction = (Direction == right ? 1.0f : -1.0f);
    for (Player.angle = -4; Player.angle < 360.0;) {
        uint64_t start = SDL_GetTicks64();
        if (!Input_GetEvent()) {
            return false;
        }
        Player.centerX += SpeedX * direction;
        SavePlayerCenterY -= SpeedY;
        Player.centerY = SavePlayerCenterY - SDL_fabsf(Player.centerX - Start) * 2 / 3;
        SpeedY -= Accelerate;
        Player.angle += 4;
        Game_DrawAll();
        SDL_RenderPresent(Renderer);
        uint64_t end = SDL_GetTicks64();
        uint64_t pause = end - start;
        SDL_Delay(pause < 12 ? 12 - pause : 0);
    }
    Player.centerX = Start + strength * direction;
    Player.centerY = StartY - SDL_fabsf((strength * direction) * 2 / 3);
    Mix_PlayChannel(-1, Music_pop, 0);
    Game_DrawAll();
    SDL_RenderPresent(Renderer);
    Player.angle = 0;
    return true;
}

static void Game_AddScoreAnimation(int score) {
    int waitTime = 100/score+1;
    while (score > 0) {
        Input_GetEvent();
        if (Keyboard[SDL_SCANCODE_SPACE] || Mouse.status) {
            Player.score += score;
            return;
        }
        Player.score++;
        Game_DrawBlockAndBackground(BlockHead.nextBlock);
        Player_Draw(1);
        char Score[20] = {"Score:"}, Combo[10] = {"Combo:"}, number[10] = {'\0'}, temp[] = {" +"};
        strcat(Score, itoa(Player.score, number, 10));
        strcat(Score, temp);
        strcat(Score, itoa(score, number, 10));
        strcat(Combo, itoa(Player.combo, number, 10));
        Display_DrawText(0, 0, Score, 0, 0, &Color.White, ScoreFont);
        Display_DrawText(0, 70, Combo, 0, 0, &Color.White, ComboFont);
        score--;
        if (Player.statue == true) {
            char *ai = "AI Mode,press Q to quit";
            Display_DrawTextByCentre(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100, ai, &Color.White, ScoreFont);
        }
        SDL_RenderPresent(Renderer);
        SDL_Delay(waitTime);
    }
}

static int Game_Check(void) {
    if (SDL_fabsf(Player.centerX - BlockHead.nextBlock->centerX) <= (BlockHead.nextBlock->w / 4)) { //到了下一个
        if (SDL_fabsf(Player.centerX - BlockHead.nextBlock->centerX) <= 5) { //踩到正中心
            Mix_PlayChannel(-1,Music_start,0);//这里懒得再找了，感觉开始这个音乐也还不错，就复用了
            Player.combo++;
        } else Player.combo = 0;
        //计算分数动画：
        Game_AddScoreAnimation((Player.combo + 1) * 2);
        return 1;//跳到下一个
    } else if ((SDL_fabsf(Player.centerX - BlockHead.nextBlock->nextBlock->centerX)) >  //掉入虚空
               (BlockHead.nextBlock->nextBlock->w / 4)) {
        if (!Game_Death()) {
            return -1;
        }
    }
    return 0;//还在当前的方块
}

static void Game_Scroll(void) {
    float direction = Player.centerX > WINDOW_HEIGHT / 2 ? -1 : 1, speed = 6;
    int delay = 20;
    for (; SDL_fabsf(Player.centerX - (float) (WINDOW_HEIGHT / 2)) >= 5.0f;) {
        Input_GetEvent();
        if (Keyboard[SDL_SCANCODE_SPACE]) {
            delay = 5;
        }
        Player.centerX += speed * direction;
        Player.centerY += speed * 2 / 3;
        struct block *cur = &BlockHead;
        while (cur->nextBlock != NULL) {
            struct block *next = cur->nextBlock;
            next->centerX += speed * direction;
            next->centerY += speed * 2 / 3;
            if ((next->centerX + next->w / 2 < 0) || (next->centerX - next->w / 2 > WINDOW_WIDTH) ||
                (next->centerY - next->w * 2 / 3 > WINDOW_HEIGHT)) {
                cur->nextBlock = next->nextBlock;
                Block_Destroy(next);
            }
            if (cur->nextBlock != NULL) cur = cur->nextBlock;
        }
        Game_DrawAll();
        SDL_RenderPresent(Renderer);
        SDL_Delay(delay);
    }
}

void Game_BlockEventCheck(void) {
    //计时，看看有没有等够三秒钟
    int kind = BlockHead.nextBlock->nextBlock->kind;
    uint64_t currentTime = SDL_GetTicks64();
    if (currentTime - checkTime < 3000 || BlockHead.nextBlock->nextBlock->isCheck == true) return;
    BlockHead.nextBlock->nextBlock->isCheck = true;
    if (kind < 5) {
        if (kind == Player.color) {
            Mix_PlayChannel(-1,Music_coin,0);
            Game_AddScoreAnimation(5);
        }
        for (int i = 0; i < 5; ++i) {
            if (i % 2) {
                Game_DrawAll();
            } else {
                Game_DrawBlockAndBackground(BlockHead.nextBlock);
                Game_DrawText();
            }
            Mix_PlayChannel(7, Music_pop, 0);
            SDL_RenderPresent(Renderer);
            SDL_Delay(200);
        }
        Mix_PlayChannel(-1, Music_changeColor, 0);
        Player_ChangeTexture(kind);
    } else if (kind == MagicCube) {
        for (int i = 0; i < 6; ++i) {
            Mix_PlayChannel(-1, Music_magicCube, 0);
            BlockHead.nextBlock->nextBlock->texture = Block_QueryTexture(kind, i);
            Game_DrawAll();
            SDL_RenderPresent(Renderer);
            SDL_Delay(100);
        }
        Game_AddScoreAnimation(10);
    } else if (kind == Luck) {
        switch (rand()%3) {
            case 0:
                if (Player.AI < 9) {
                    Mix_PlayChannel(-1,Music_robot,0);
                    Player.AI++;
                    Game_DrawAll();
                    Display_DrawPictureByCentre(Player.centerX, Player.centerY - Player.h * 2, 70, 70, 0, NULL, robot);
                    SDL_RenderPresent(Renderer);
                    SDL_Delay(300);
                    break;
                }
            case 1:
                if (Player.star < 9) {
                    Mix_PlayChannel(-1,Music_star,0);
                    Player.star++;
                    Game_DrawAll();
                    Display_DrawPictureByCentre(Player.centerX, Player.centerY - Player.h * 2, 70, 70, 0, NULL, star);
                    SDL_RenderPresent(Renderer);
                    SDL_Delay(300);
                    break;
                }
            case 2:
                Mix_PlayChannel(-1,Music_coin,0);
                Game_DrawAll();
                Display_DrawPictureByCentre(Player.centerX,Player.centerY-Player.h*2,70,70,0,NULL,coin);
                SDL_RenderPresent(Renderer);
                SDL_Delay(300);
                Game_AddScoreAnimation(20);
                break;
        }
    } else if (kind == Music) {
        switch (rand()%3) {
            case 0:
                Mix_PlayChannel(-1,Music_music1,0);
                break;
            case 1:
                Mix_PlayChannel(-1,Music_music2,0);
                break;
            case 2:Mix_PlayChannel(-1,Music_music3,0);
                break;
        }
        Game_DrawAll();
        Display_DrawPictureByCentre(Player.centerX,Player.centerY-Player.h*2,150,150,0,NULL,loveMusic);
        SDL_RenderPresent(Renderer);
        SDL_Delay(300);
        Game_AddScoreAnimation(15);
    } else if (kind == Life) {
        Mix_PlayChannel(-1,Music_star,0);
        if(Player.lives <3) Player.lives++;
        Game_DrawAll();
        Display_DrawPictureByCentre(Player.centerX,Player.centerY-Player.h*2,75,75,0,NULL,heart);
        SDL_RenderPresent(Renderer);
        SDL_Delay(300);
    }
    Input_Clear();
}

static bool Game_Death(void) {
    for (int i = 0;(float)i <BlockHead.nextBlock->h - BlockHead.nextBlock->w * 2 / 3 ;i++ ) {
        Player.centerY++;
        if ( (Direction == right && Player.centerX < BlockHead.nextBlock->centerX) ||(Direction==left && Player.centerX > BlockHead.nextBlock->centerX)) {
            Game_DrawBlockAndBackground(BlockHead.nextBlock);
            Player_Draw(1);
            Game_DrawText();
        } else {
            Game_DrawBlockAndBackground(BlockHead.nextBlock->nextBlock);
            Player_Draw(1);
            Block_Draw(BlockHead.nextBlock,1);
            Game_DrawText();
        }
        SDL_Delay(2);
        SDL_RenderPresent(Renderer);
    }
    SDL_Delay(200);
    Player.lives--;
    if (Player.lives > 0) {
        Mix_PlayChannel(-1, Music_failed, 0);
        Player.centerX = BlockHead.nextBlock->nextBlock->centerX;
        Player.centerY = BlockHead.nextBlock->nextBlock->centerY;
        for (int i = 0; i < 5; ++i) {
            if (i % 2) {
                Game_DrawAll();
            } else {
                Game_DrawBlockAndBackground(BlockHead.nextBlock);
                Game_DrawText();
            }
            SDL_RenderPresent(Renderer);
            SDL_Delay(200);
        }
        return true;
    } else {
        Mix_PlayChannel(-1, Music_heart, 0);
        printf("GG\n");
        return false;
    }
}


static void Game_Init() {
    Block_Init();
    Player_Init();
    ScoreFont = TTF_OpenFont("font/COOPBL.TTF", 70);
    ComboFont = TTF_OpenFont("font/COOPBL.TTF", 70);
    CheatFont = TTF_OpenFont("font/COOPBL.TTF", 35);
    if (ScoreFont == NULL || ComboFont == NULL || CheatFont) {
        SDL_Log("SDL_Menu_Init_LoadFont failed: %s", SDL_GetError());
    }
    StrengthOuter = IMG_LoadTexture(Renderer, "picture/StrengthOuter.png");
    heart = IMG_LoadTexture(Renderer, "picture/heart.png");
    starIn = IMG_LoadTexture(Renderer,"picture/starIn.png");
    starOut = IMG_LoadTexture(Renderer,"picture/starOut.png");
    robotIn = IMG_LoadTexture(Renderer,"picture/robotIn.png");
    robotOut = IMG_LoadTexture(Renderer,"picture/robotOut.png");
    coin = IMG_LoadTexture(Renderer,"picture/coin.png");
    gearOut = IMG_LoadTexture(Renderer, "picture/gearOut.png");
    gearIn = IMG_LoadTexture(Renderer, "picture/gearIn.png");
    loveMusic = IMG_LoadTexture(Renderer,"picture/loveMusic.png");
    srand(time(NULL));
    gear = gearOut;
    robot = robotOut;
    star = starOut;
}

static void Game_Quit() {
    TTF_CloseFont(ComboFont);
    TTF_CloseFont(ScoreFont);
    TTF_CloseFont(CheatFont);
    SDL_DestroyTexture(StrengthOuter);
    SDL_DestroyTexture(heart);
    SDL_DestroyTexture(starIn);
    SDL_DestroyTexture(starOut);
    SDL_DestroyTexture(robotIn);
    SDL_DestroyTexture(robotOut);
    SDL_DestroyTexture(coin);
    SDL_DestroyTexture(gearOut);
    SDL_DestroyTexture(gearIn);
    SDL_DestroyTexture(loveMusic);
    Player_Quit();
    Block_Quit();
}