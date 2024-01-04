//
// Created by 15355 on 2023/12/24.
//
#ifndef BOTTLEFLIP_COMMON_H
#define BOTTLEFLIP_COMMON_H

#include "SDL2/SDL.h"
#include <stdbool.h>

#define PROJECT_NAME "BottleFlip" //项目名称
#define WINDOW_WIDTH 800.0 //窗口宽度
#define WINDOW_HEIGHT 870.0 //窗口高度
#define Fps 60 //刷新率

//全局变量
extern SDL_Window *Window;//窗口
extern SDL_Surface *Surface;//表面
extern SDL_Renderer *Renderer;//渲染器
extern bool Keyboard[512];//记录键盘按键状态 0表示没有按下 1表示按下
extern struct mouse{
    float x;//记录鼠标X坐标
    float y;//记录鼠标y坐标
    int clicks;//记录鼠标点击次数
    bool status;//false 表示没有按下 true表示按下
    bool move;//false 表示没有移动 true移动了
}Mouse;

#endif //BOTTLEFLIP_COMMON_H
