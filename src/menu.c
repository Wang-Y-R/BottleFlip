//
// Created by 15355 on 2023/12/24.
//

#include "menu.h"
#include "game.h"
#include "setting.h"
#include "block.h"
#include "player.h"
#include <SDL2/SDL_mixer.h>
#include "utils/input.h"
#include "utils/display.h"
#define  Button_Width 370.0
#define  Button_Height 70.0
//标题
char *title = "BottleFlip\0";
static TTF_Font *FontTitle;
float titleY;
//按钮类型
enum Select{
    Start,Continue,setting,Quit
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
        Button_Width,Button_Height,"New Game",&Color.MintCream,

        false, WINDOW_WIDTH/2-Button_Width/2,WINDOW_HEIGHT/2-Button_Height,
        Button_Width,Button_Height,"Continue",&Color.MintCream,

        false, WINDOW_WIDTH/2-Button_Width/2,WINDOW_HEIGHT/2-Button_Height,
        Button_Width,Button_Height,"Setting",&Color.MintCream,

        false,WINDOW_WIDTH/2-Button_Width/2,WINDOW_HEIGHT/2+Button_Height,
        Button_Width,Button_Height,"Quit!!!!!",&Color.MintCream,
};
//预设字体
static TTF_Font *Font;
//用到的音频
extern Mix_Chunk *Music_buttonSelect;
extern Mix_Chunk *Music_start;
//菜单初始化
static void Menu_Init(void){
    CurrentButton = 0;
    Font = TTF_OpenFont("font/COOPBL.TTF",60);
    FontTitle = TTF_OpenFont("font/COOPBL.TTF",150);
    if (Font == NULL) {
        SDL_Log("SDL_Menu_Init_LoadFont failed: %s", SDL_GetError());
    }
    button[CurrentButton].statue = 1;
    for (int i = 0; i < 4; ++i) {
        button[i].fRect.x = WINDOW_WIDTH/2-Button_Width/2;
        button[i].fRect.y = WINDOW_HEIGHT/2-Button_Height + i * 2 * Button_Height;
    }
    titleY = 200;
}
//菜单主循环
void Menu(void) {
    Menu_Init();
    //核心循环
    while (true) {
        uint64_t start = SDL_GetTicks64();//计时起点
        if(!Input_GetEvent()){//玩家是否退出，是的话直接返回main函数进行退出
            Menu_Quit();
            return;
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
            if (!Menu_Select()) return;
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
            Menu_Draw();                                                //鼠标立即反馈，不进行延时
            continue;
        } else if (Mouse.status) {                                      //玩家点击鼠标
            SDL_FPoint fPoint= {Mouse.x,Mouse.y};
            if (SDL_PointInFRect(&fPoint,&button[CurrentButton].fRect)) {
                if (!Menu_Select()) return;
            }
        }
        Menu_Draw();
        uint64_t end = SDL_GetTicks64();                                //计时终点
        SDL_Delay( (end - start) < (150) ? 150 : 0);
        Input_Clear();
    }
}


static void Menu_Draw(void){                                            //绘制菜单界面
    SDL_SetRenderDrawColor(Renderer,BackgroundColor[CurrentColor].r,BackgroundColor[CurrentColor].g,BackgroundColor[CurrentColor].b,BackgroundColor[CurrentColor].a);
    SDL_RenderClear(Renderer);
    for (int i = 0; i < 4; ++i) {
        if (!button[i].statue){
            Display_FillFRect(&button[i].fRect, button[i].color);
        } else Display_FillFRect(&button[i].fRect, &Color.Cornsilk);
        Display_DrawTextByCentre(button[i].fRect.x + button[i].fRect.w / 2, button[i].fRect.y + button[i].fRect.h / 2,
                                 button[i].content, &Color.Gray, Font);
    }
    Display_DrawTextByCentre(WINDOW_WIDTH/2,titleY,title,&Color.White,FontTitle);
    SDL_RenderPresent(Renderer);
}

static bool Menu_Select(void) {                                         //玩家选择选项事件
    switch (CurrentButton) {
        case Start:
            Player_Clear();
            Block_Clear();
            Mix_PlayChannel(-1,Music_start,0);
            if (!Menu_StartAnimation()) {
                Menu_Quit();
                return false;                                   //游戏开始的菜单动画
            }
            if (!Game()) {
                Menu_Quit();
                return false;                                  //加入游戏
            }
            break;
        case Continue:
            if (BlockHead.nextBlock == NULL) {
                char content[30]= {"Start a new game first !"};
                Menu_Draw();
                Display_DrawTextByCentre(WINDOW_WIDTH/2,450, content,&Color.Red,Font);
                SDL_RenderPresent(Renderer);
                SDL_Delay(500);
            } else if (!Game()) {
                Menu_Quit();
                return false;                                  //加入游戏
            }
            break;
        case Quit:
            Menu_Quit();
            return false;
        case setting:
            Setting();
            break;
        default:
            break;
    }
    Input_Clear();
    return true;
}

static bool Menu_StartAnimation(void) {                                //游戏开始的菜单动画
    Input_Clear();
    float speed = -20,accelerate = 1,h = 0;
    while (titleY+h <= WINDOW_HEIGHT) {
        if(!Input_GetEvent()){
            return false;
        }
        SDL_SetRenderDrawColor(Renderer,BackgroundColor[CurrentColor].r,BackgroundColor[CurrentColor].g,BackgroundColor[CurrentColor].b,BackgroundColor[CurrentColor].a);
        SDL_RenderClear(Renderer);
        for (int i = 0; i < 4; ++i) {
            if (!button[i].statue){
                Display_FillFRectByCentre(button[i].fRect.x + button[i].fRect.w / 2,
                                          button[i].fRect.y + button[i].fRect.h / 2 + h, button[i].fRect.w,
                                          button[i].fRect.h, button[i].color);
            } else
                Display_FillFRectByCentre(button[i].fRect.x + button[i].fRect.w / 2,
                                          button[i].fRect.y + button[i].fRect.h / 2 + h, button[i].fRect.w,
                                          button[i].fRect.h, &Color.Cornsilk);
            Display_DrawTextByCentre(button[i].fRect.x + button[i].fRect.w / 2,
                                     button[i].fRect.y + button[i].fRect.h / 2 + h,
                                     button[i].content, &Color.Gray, Font);
        }
        Display_DrawTextByCentre(WINDOW_WIDTH/2,titleY+h,title,&Color.White,FontTitle);
        SDL_RenderPresent(Renderer);
        h+=speed;
        speed+=accelerate;
        SDL_Delay(1000/Fps);
        if (Mouse.status == true) break;
    }
    return true;
}

void Menu_Quit() {
    TTF_CloseFont(Font);
    TTF_CloseFont(FontTitle);
}