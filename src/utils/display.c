//
// Created by 15355 on 2023/12/24.
//

#include "display.h"

void Display_DrawPicture(float x, float y, float w,float h,double angle, SDL_Rect *srcRect, SDL_Texture *texture) {
    if (w == 0 && h == 0) {
        int tempW,tempH;
        SDL_QueryTexture(texture, NULL, NULL, &tempW, &tempH);
        w = (float)tempW,h = (float)tempH;
    }
    SDL_FRect target = {x, y, (float)w, (float)h};
    SDL_RenderCopyExF(Renderer, texture, srcRect, &target, angle, NULL, 0);
}

void Display_DrawPictureByCentre(float centreX, float centreY, float w,float h,double angle, SDL_Rect *srcRect, SDL_Texture *texture) {
    if (w == 0 && h == 0) {
        int tempW,tempH;
        SDL_QueryTexture(texture, NULL, NULL, &tempW, &tempH);
        w = (float)tempW,h = (float)tempH;
    }
    SDL_FRect target = {centreX - w / 2, centreY - h / 2, (float)w, (float)h};
    SDL_RenderCopyExF(Renderer, texture, srcRect, &target, angle, NULL, 0);
}

void Display_FillFRect(SDL_FRect *rect, SDL_Color *color) {
    SDL_SetRenderDrawColor(Renderer,color->r,color->g,color->b,color->a);
    SDL_RenderFillRectF(Renderer,rect);
}

void Display_DrawText(float x, float y, const char *content,float w,float h,SDL_Color *color, TTF_Font *Font) {
    Surface = TTF_RenderUTF8_Blended(Font, content, *color);
    SDL_Texture *TextTexture = SDL_CreateTextureFromSurface(Renderer, Surface);
    SDL_FRect Text;
    if (w==0 && h==0){
        int tempW,tempH;
        SDL_QueryTexture(TextTexture, NULL, NULL, &tempW, &tempH);
        w = (float)tempW,h = (float)tempH;
    }
    Text.w = w;
    Text.h = h;
    Text.x = x;
    Text.y = y;
    SDL_RenderCopyF(Renderer, TextTexture, NULL, &Text);
    SDL_FreeSurface(Surface);
    SDL_DestroyTexture(TextTexture);
}

void Display_DrawTextByCentre(float centreX, float centreY, const char *content, SDL_Color *color, TTF_Font *Font) {
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
    SDL_FreeSurface(Surface);
    SDL_DestroyTexture(TextTexture);
}

void Display_FillFRectByCentre(float centerX, float centerY, float w, float h, SDL_Color *color) {
    SDL_FRect rect = {centerX - w / 2, centerY - h / 2, w, h};
    SDL_SetRenderDrawColor(Renderer,color->r,color->g,color->b,color->a);
    SDL_RenderFillRectF(Renderer,&rect);
}