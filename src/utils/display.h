//
// Created by 15355 on 2023/12/24.
//



#ifndef BOTTLEFLIP_DISPLAY_H
#define BOTTLEFLIP_DISPLAY_H
#include "../common.h"
#include "SDL2/SDL_ttf.h"
/**
  *  @brief  渲染图片
  *  @param  x:目标坐标x(左上角)
  *  @param  y:目标坐标y(左上角)
  *  @param  angle:角度
  *  @param  rect:图片渲染区块
  *  @param  texture:目标图片纹理
*/
void Display_Draw_Picture(float x,float y,double angle,SDL_Rect *rect,SDL_Texture *texture);
/**
  *  @brief  渲染图片
  *  @param rect:矩形
  *  @param color:矩形颜色
*/
void Display_Fill_FRect(SDL_FRect *rect, SDL_Color *color);
/**
  *  @brief  渲染图片
  *  @param  centreX:目标坐标x(中心)
  *  @param  centreY:目标坐标y(中心)
*/
void Display_Draw_Text(float centreX, float centreY, const char *content, SDL_Color *color, TTF_Font *Font);

#endif //BOTTLEFLIP_DISPLAY_H
