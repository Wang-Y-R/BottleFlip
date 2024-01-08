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
  *  @param  w;目标宽度
  *  @param  h:目标高度 两个都为0的话则根据纹理大小自动计算
  *  @param  angle:角度
  *  @param  srcRect:图片渲染区块 Null表示全部
  *  @param  texture:目标图片纹理
*/
void Display_DrawPicture(float x, float y, float w,float h,double angle, SDL_Rect *srcRect, SDL_Texture *texture);

/**
  *  @brief  通过中心点计算来渲染图形,如果w，h都为0,则根据图片大小计算宽度长度
  *  @param  centerX:目标坐标x(中心)
  *  @param  centerY:目标坐标y(中心)
  *  @param w:宽
  *  @param h：高
  *  @param angle:角度
  *  @param srcRect:图片渲染区块 Null表示全部
  *  @param texture：目标图片纹理
*/
void Display_DrawPictureByCentre(float centreX, float centreY, float w,float h,double angle, SDL_Rect *srcRect, SDL_Texture *texture);

/**
  *  @brief  渲染矩形
  *  @param rect:矩形
  *  @param color:矩形颜色
*/
void Display_FillFRect(SDL_FRect *rect, SDL_Color *color);

/**
  *  @brief  通过中心点计算来渲染矩形,如果w，h都为0则根据字体大小计算宽度长度
  *  @param  centerX:目标坐标x(中心)
  *  @param  centerY:目标坐标y(中心)
  *  @param w:矩形宽
  *  @param h：矩形高
  *  @param color:颜色指针
*/
void Display_FillFRectByCentre(float centerX, float centerY, float w, float h, SDL_Color *color);

/**
  *  @brief  提供文本中心点渲染文本
  *  @param  centreX:目标坐标x(中心)
  *  @param  centreY:目标坐标y(中心)
  *  @param content:文本
  *  @param color:颜色指针
  *  @param Font:字体指针
*/

void Display_DrawTextByCentre(float centreX, float centreY, const char *content, SDL_Color *color, TTF_Font *Font);

/**
  *  @brief  提供文本左上角坐标渲染文本，如果w，h都为0则根据字体大小计算宽度长度
  *  @param X:坐标x
  *  @param Y:坐标y
  *  @param w;目标宽度
  *  @param h:目标高度 两个都为0的话则根据纹理大小自动计算
  *  @param content:文本
  *  @param color:颜色指针
  *  @param Font:字体指针
*/
void Display_DrawText(float x, float y, const char *content,float w,float h,SDL_Color *color, TTF_Font *Font);

#endif //BOTTLEFLIP_DISPLAY_H
