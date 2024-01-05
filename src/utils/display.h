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
  *  @param  srcRect:图片渲染区块 Null表示全部
  *  @param  texture:目标图片纹理
*/
void Display_DrawPicture(float x, float y, double angle, SDL_Rect *srcRect, SDL_Texture *texture);
/**
  *  @brief  渲染矩形
  *  @param rect:矩形
  *  @param color:矩形颜色
*/
void Display_FillFRect(SDL_FRect *rect, SDL_Color *color);
/**
  *  @brief  渲染文本
  *  @param  centreX:目标坐标x(中心)
  *  @param  centreY:目标坐标y(中心)
  *  @param content:文本
  *  @param color:颜色指针
  *  @param Font:字体指针
*/
void Display_DrawText(float centreX, float centreY, const char *content, SDL_Color *color, TTF_Font *Font);
/**
  *  @brief  通过中心点计算来渲染矩形
  *  @param  centerX:目标坐标x(中心)
  *  @param  centerY:目标坐标y(中心)
  *  @param w:矩形宽
  *  @param h：矩形高
  *  @param color:颜色指针
*/
void Display_FillFRectByCenter(float centerX, float centerY, float w, float h, SDL_Color *color);
#endif //BOTTLEFLIP_DISPLAY_H
