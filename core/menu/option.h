#pragma once

#include "../../config/menu.h"

struct menu;

struct menuOption
{
    struct menu *submenu;
    struct menuOption *next;
    struct menuOption *prev;

    unsigned char data[MENU_CONFIG_DATASIZE];
};