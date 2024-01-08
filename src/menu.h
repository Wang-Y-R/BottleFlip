//
// Created by 15355 on 2023/12/24.
//
#include "common.h"



#ifndef BOTTLEFLIP_MENU_H
#define BOTTLEFLIP_MENU_H

void Menu(void);

static void Menu_Init(void);

static void Menu_Draw(void);

static bool Menu_Select(void);

static bool Menu_StartAnimation(void);

void Menu_Quit(void);

#endif //BOTTLEFLIP_MENU_H
