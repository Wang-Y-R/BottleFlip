//
// Created by 15355 on 2023/12/24.
//

#include "display.h"

void Display_Draw_Picture(const float x,const float y,const double angle,SDL_Rect *rect,SDL_Texture *texture) {
    SDL_FRect target = {x,y};
    SDL_QueryTexture(texture,NULL,NULL,&target.w,&target.h);
    SDL_RenderCopyExF(Renderer, texture, rect, &target, angle, NULL, 0);
}

void Display_Fill_FRect(SDL_FRect *rect, SDL_Color *color) {
    SDL_SetRenderDrawColor(Renderer,color->r,color->g,color->b,color->a);
    SDL_RenderFillRectF(Renderer,rect);
}

void Display_Draw_Text(const float centreX, const float centreY, const char *content, SDL_Color *color, TTF_Font *Font) {
    SDL_FRect Text;
    int w,h;
    Surface = TTF_RenderUTF8_Blended(Font, content, *color);
    SDL_Texture *TextTexture = SDL_CreateTextureFromSurface(Renderer, Surface);
    SDL_QueryTexture(TextTexture, NULL, NULL, &w,&h);
    Text.w = (float )w;
    Text.h = (float )h;
    Text.x = centreX-Text.w/2;
    Text.y = centreY-Text.h/2;
    SDL_RenderCopyF(Renderer, TextTexture, NULL, &Text);
    SDL_DestroyTexture(TextTexture);
}
