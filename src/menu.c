//
// Created by 15355 on 2023/12/24.
//

#include "menu.h"
#include "utils/input.h"
#include "utils/display.h"

#define  Button_Width 300.0
#define  Button_Height 70.0

typedef struct {
    bool statue; //是否选中
    SDL_FRect fRect;
    char *content;  //内容
    SDL_Color *color;
}Button;
int CurrentButton=0;
static SDL_Color Color_MistyRose= {255, 228, 225,255},Color_MintCream= {245, 255, 250,255},
                 Color_Gray = {119 , 136, 153, 255},Color_Cornsilk={255, 248, 220,255};

static Button button[2]={
        false, WINDOW_WIDTH/2-Button_Width/2,WINDOW_HEIGHT/2-Button_Height,
        Button_Width,Button_Height,"Start",&Color_MintCream,

        false,WINDOW_WIDTH/2-Button_Width/2,WINDOW_HEIGHT/2+Button_Height,
        Button_Width,Button_Height,"Quit!!!!!",&Color_MintCream,
};

TTF_Font *Font;

static void Menu_Init(void){
    Font = TTF_OpenFont("font/COOPBL.TTF",60);
    if (Font == NULL) {
        SDL_Log("SDL_Menu_Init_LoadFont failed: %s", SDL_GetError());
    }
    button[CurrentButton].statue = 1;
}

void Menu(void) {
    Menu_Init();
    while (1) {
        uint64_t start = SDL_GetTicks64();
        if(!Input_GetEvent()){
            break;
        }
        if (Keyboard[SDL_SCANCODE_ESCAPE]) {
            break;
        } else if (Keyboard[SDL_SCANCODE_UP] || Keyboard[SDL_SCANCODE_W]) {
            button[CurrentButton].statue = 0;
            CurrentButton = (CurrentButton+1) % 2;
            button[CurrentButton].statue = 1;
        } else if (Keyboard[SDL_SCANCODE_SPACE]) {
            if (CurrentButton == 1) break;
        } else if (Mouse.move) {
            SDL_FPoint fPoint= {Mouse.x,Mouse.y};
            for (int i = 0; i < 2; ++i) {
                if (SDL_PointInFRect(&fPoint,&button[i].fRect)) {
                    button[CurrentButton].statue = false;
                    CurrentButton = i;
                    button[CurrentButton].statue = true;
                    break;
                }
            }
            Menu_Draw();
            continue;
        } else if (Mouse.status) {
            SDL_FPoint fPoint= {Mouse.x,Mouse.y};
            if (SDL_PointInFRect(&fPoint,&button[CurrentButton].fRect)) {
                if (CurrentButton == 1) break;
            }
        } else {
            Menu_Draw();
            continue;
        }
        Menu_Draw();
        uint64_t end = SDL_GetTicks64();
        SDL_Delay( (end - start) < (150) ? 150 : 0);
    }
}


static void Menu_Draw(void){
    SDL_SetRenderDrawColor(Renderer,Color_MistyRose.r,Color_MistyRose.g,Color_MistyRose.b,Color_MistyRose.a);
    SDL_RenderClear(Renderer);
    for (int i = 0; i < 2; ++i) {
        if (!button[i].statue){
            Display_Fill_FRect(&button[i].fRect, button[i].color);
        } else Display_Fill_FRect(&button[i].fRect, &Color_Cornsilk);
        Display_Draw_Text(button[i].fRect.x+button[i].fRect.w/2,button[i].fRect.y+button[i].fRect.h/2, button[i].content, &Color_Gray, Font);
    }
    SDL_RenderPresent(Renderer);
}
