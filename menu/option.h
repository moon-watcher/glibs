#pragma once

#include "config/config.h"

typedef struct menu menu;
typedef struct menuOption menuOption;

typedef struct menuOption
{
    menu *submenu;
    menuOption *next;
    menuOption *prev;

    unsigned char data[MENU_CONFIG_DATASIZE];
} menuOption;