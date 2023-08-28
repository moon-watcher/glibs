#pragma once

#include "config.h"

typedef struct menu_t menu_t;
typedef struct menuOption_t menuOption_t;

typedef struct menuOption_t
{
    menu_t *submenu;
    menuOption_t *next;
    menuOption_t *prev;

    unsigned char data[CONFIG_MENU_DATASIZE];
} menuOption_t;