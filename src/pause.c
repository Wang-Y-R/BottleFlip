//
// Created by 15355 on 2024/1/8.
//

#include "pause.h"
#include "player.h"
#include "setting.h"
#include <SDL2/SDL_mixer.h>
#include "utils/input.h"
#include "utils/display.h"
#define  Button_Width 300.0
#define  Button_Height 70.0
//按钮类型
enum Select{
    Continue,setting,AIMode,BackMenu
};
//按钮相关内容
typedef struct {
    bool statue; //是否选中
    SDL_FRect fRect;//矩形
    char *content;  //内容
    SDL_Color *color;//颜色
}Button;
static int CurrentButton=0;//当前选择按钮
//预设的按钮
static Button button[4]={
        false, WINDOW_WIDTH/2-Button_Width/2,WINDOW_HEIGHT/2-Button_Height,
        Button_Width,Button_Height,"Continue",&Color.MintCream,

        false,WINDOW_WIDTH/2-Button_Width/2,WINDOW_HEIGHT/2+Button_Height,
        Button_Width,Button_Height,"Setting",&Color.MintCream,

        false,WINDOW_WIDTH/2-Button_Width/2,WINDOW_HEIGHT/2+Button_Height,
        Button_Width,Button_Height,"AI Mode",&Color.MintCream,

        false,WINDOW_WIDTH/2-Button_Width/2,WINDOW_HEIGHT/2+Button_Height,
        Button_Width,Button_Height,"Menu",&Color.MintCream,
};
//预设字体
static TTF_Font *Font;
//用到的音频
extern Mix_Chunk *Music_buttonSelect;
extern Mix_Chunk *Music_start;
//返回游戏还是菜单
bool backGame = true;

static void Pause_Init(void){
    CurrentButton = 0;
    Font = TTF_OpenFont("font/COOPBL.TTF",60);
    if (Font == NULL) {
        SDL_Log("SDL_Pause_Init_LoadFont failed: %s", SDL_GetError());
    }
    for (int i = 0; i < 4; ++i) {
        button[i].fRect.x = WINDOW_WIDTH/2-Button_Width/2;
        button[i].fRect.y = WINDOW_HEIGHT/2-Button_Height * 3 + i * 2 * Button_Height;
    }
    backGame = true;
}

bool Pause(void) {
    Pause_Init();
    //核心循环
    while (true) {
        uint64_t start = SDL_GetTicks64();//计时起点
        if(!Input_GetEvent()){//玩家是否退出，是的话直接返回main函数进行退出
            Pause_Quit();
            return false;
        }
        if (Keyboard[SDL_SCANCODE_DOWN] || Keyboard[SDL_SCANCODE_S]) { //玩家向下选择
            button[CurrentButton].statue = false;
            CurrentButton = (CurrentButton+1) % 4;
            button[CurrentButton].statue = true;
            Mix_PlayChannel(-1,Music_buttonSelect,0);
        } else if (Keyboard[SDL_SCANCODE_UP]||Keyboard[SDL_SCANCODE_W]) { //向上选择
            button[CurrentButton].statue = false;
            CurrentButton = (CurrentButton+4-1) % 4;
            button[CurrentButton].statue = true;
            Mix_PlayChannel(-1,Music_buttonSelect,0);
        } else if (Keyboard[SDL_SCANCODE_SPACE]||Keyboard[SDL_SCANCODE_KP_ENTER]) {  //玩家按空格或回车确认
            if (!Pause_Select()) {
                Pause_Quit();
                if (!backGame) return false;
                else return true;
            }
        } else if (Mouse.move) {                                        //玩家移动鼠标
            SDL_FPoint fPoint= {Mouse.x,Mouse.y};
            for (int i = 0; i < 4; ++i) {
                if (SDL_PointInFRect(&fPoint,&button[i].fRect)) {
                    if (CurrentButton != i ) {
                        button[CurrentButton].statue = false;
                        CurrentButton = i;
                        button[CurrentButton].statue = true;
                        Mix_PlayChannel(-1, Music_buttonSelect, 0);
                        break;
                    }
                }
            }
            Pause_Draw();                                                //鼠标立即反馈，不进行延时
            continue;
        } else if (Mouse.status) {                                      //玩家点击鼠标
            SDL_FPoint fPoint= {Mouse.x,Mouse.y};
            if (SDL_PointInFRect(&fPoint,&button[CurrentButton].fRect)) {
                if (!Pause_Select()) {
                    Pause_Quit();
                    if (!backGame) return false;
                    else return true;
                }
            }
        }
        Pause_Draw();
        uint64_t end = SDL_GetTicks64();                                //计时终点
        SDL_Delay( (end - start) < (100) ? 100 : 0);
        Input_Clear();
    }
}

static void Pause_Draw(void){                                            //绘制菜单界面
    SDL_SetRenderDrawColor(Renderer,BackgroundColor[CurrentColor].r,BackgroundColor[CurrentColor].g,BackgroundColor[CurrentColor].b,BackgroundColor[CurrentColor].a);
    SDL_RenderClear(Renderer);
    for (int i = 0; i < 4; ++i) {
        if (!button[i].statue){
            Display_FillFRect(&button[i].fRect, button[i].color);
        } else Display_FillFRect(&button[i].fRect, &Color.Cornsilk);
        Display_DrawTextByCentre(button[i].fRect.x + button[i].fRect.w / 2, button[i].fRect.y + button[i].fRect.h / 2,
                                 button[i].content, &Color.Gray, Font);
    }
    SDL_RenderPresent(Renderer);
}

static bool Pause_Select(void) {                                         //玩家选择选项事件
    switch (CurrentButton) {
        case Continue:
            return false;
        case setting:
            Setting();
            break;
        case AIMode:
            Player.statue = true;
            return false;
        case BackMenu:
            backGame = true;
            return false;
        default:
            break;
    }
    Input_Clear();
    return true;
}


void Pause_Quit() {
    TTF_CloseFont(Font);
}