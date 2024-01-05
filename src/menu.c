//
// Created by 15355 on 2023/12/24.
//

#include "menu.h"
#include "game.h"
#include "utils/input.h"
#include "utils/display.h"


#define  Button_Width 300.0
#define  Button_Height 70.0
//按钮类型
enum Select{
    Start,Quit
};
//按钮相关内容
typedef struct {
    bool statue; //是否选中
    SDL_FRect fRect;//矩形
    char *content;  //内容
    SDL_Color *color;//颜色
}Button;
int CurrentButton=0;//当前选择按钮
//预设的颜色

//预设的按钮
static Button button[2]={
        false, WINDOW_WIDTH/2-Button_Width/2,WINDOW_HEIGHT/2-Button_Height,
        Button_Width,Button_Height,"Start",&Color.MintCream,

        false,WINDOW_WIDTH/2-Button_Width/2,WINDOW_HEIGHT/2+Button_Height,
        Button_Width,Button_Height,"Quit!!!!!",&Color.MintCream,
};
//预设字体
TTF_Font *Font;
//菜单初始化
static void Menu_Init(void){
    Font = TTF_OpenFont("font/COOPBL.TTF",60);
    if (Font == NULL) {
        SDL_Log("SDL_Menu_Init_LoadFont failed: %s", SDL_GetError());
    }
    button[CurrentButton].statue = 1;
}
//菜单主循环
bool Menu(void) {
    //是否第一次加载菜单，是的话则初始化
    static int init = 1;
    if (init) {
        Menu_Init();
        init = 0;
    }
    //核心循环
    while (true) {
        uint64_t start = SDL_GetTicks64();              //计时起点
        if(!Input_GetEvent()){                          //玩家是否退出，是的话直接返回main函数进行退出
            return false;
        }
        if (Keyboard[SDL_SCANCODE_DOWN] || Keyboard[SDL_SCANCODE_S]) { //玩家向下选择
            button[CurrentButton].statue = false;
            CurrentButton = (CurrentButton+1) % 2;
            button[CurrentButton].statue = true;
        } else if (Keyboard[SDL_SCANCODE_UP]||Keyboard[SDL_SCANCODE_W]) { //向上选择
            button[CurrentButton].statue = false;
            CurrentButton = (CurrentButton+2-1) % 2;
            button[CurrentButton].statue = true;
        } else if (Keyboard[SDL_SCANCODE_SPACE]||Keyboard[SDL_SCANCODE_KP_ENTER]) {  //玩家按空格或回车确认
            if (!Menu_Select()) return false;
        } else if (Mouse.move) {                                        //玩家移动鼠标
            SDL_FPoint fPoint= {Mouse.x,Mouse.y};
            for (int i = 0; i < 2; ++i) {
                if (SDL_PointInFRect(&fPoint,&button[i].fRect)) {
                    button[CurrentButton].statue = false;
                    CurrentButton = i;
                    button[CurrentButton].statue = true;
                    break;
                }
            }
            Menu_Draw();                                                //鼠标立即反馈，不进行延时
            continue;
        } else if (Mouse.status) {                                      //玩家点击鼠标
            SDL_FPoint fPoint= {Mouse.x,Mouse.y};
            if (SDL_PointInFRect(&fPoint,&button[CurrentButton].fRect)) {
                if (!Menu_Select()) return false;
            }
        } else {                                                        //无任何输入不进行延时
            Menu_Draw();
            continue;
        }
        Menu_Draw();
        uint64_t end = SDL_GetTicks64();                                //计时终点
        SDL_Delay( (end - start) < (150) ? 150 : 0);
    }
}


static void Menu_Draw(void){                                            //绘制菜单界面
    SDL_SetRenderDrawColor(Renderer,Color.MistyRose.r,Color.MistyRose.g,Color.MistyRose.b,Color.MistyRose.a);
    SDL_RenderClear(Renderer);
    for (int i = 0; i < 2; ++i) {
        if (!button[i].statue){
            Display_FillFRect(&button[i].fRect, button[i].color);
        } else Display_FillFRect(&button[i].fRect, &Color.Cornsilk);
        Display_DrawText(button[i].fRect.x + button[i].fRect.w / 2, button[i].fRect.y + button[i].fRect.h / 2,
                         button[i].content, &Color.Gray, Font);
    }
    SDL_RenderPresent(Renderer);
}

static bool Menu_Select(void) {                                         //玩家选择选项事件
    switch (CurrentButton) {
        case Start:
            if (!Menu_StartAnimation()) return false;                  //游戏开始的菜单动画
            if (!Game()) return false;                                                     //加入游戏
            break;
        case Quit:
            return false;
        default:
            break;
    }
    Input_Clear();
    return true;
}

static bool Menu_StartAnimation(void) {                                //游戏开始的菜单动画
    float speed = -20,accelerate = 1,h = 0;
    while (button[0].fRect.y-button[0].fRect.h+h <= WINDOW_HEIGHT) {
        if(!Input_GetEvent()){
            return false;
        }
        SDL_SetRenderDrawColor(Renderer,Color.MistyRose.r,Color.MistyRose.g,Color.MistyRose.b,Color.MistyRose.a);
        SDL_RenderClear(Renderer);
        for (int i = 0; i < 2; ++i) {
            if (!button[i].statue){
                Display_FillFRectByCenter(button[i].fRect.x + button[i].fRect.w / 2,
                                          button[i].fRect.y + button[i].fRect.h / 2 + h, button[i].fRect.w,
                                          button[i].fRect.h, button[i].color);
            } else
                Display_FillFRectByCenter(button[i].fRect.x + button[i].fRect.w / 2,
                                          button[i].fRect.y + button[i].fRect.h / 2 + h, button[i].fRect.w,
                                          button[i].fRect.h, &Color.Cornsilk);
            Display_DrawText(button[i].fRect.x + button[i].fRect.w / 2, button[i].fRect.y + button[i].fRect.h / 2 + h,
                             button[i].content, &Color.Gray, Font);
        }
        SDL_RenderPresent(Renderer);
        h+=speed;
        speed+=accelerate;
        SDL_Delay(1000/Fps);
    }
    SDL_Delay(1000);
    return true;
}