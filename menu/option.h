#pragma once

#include "config.h"

struct menu;

struct menuOption
{
    struct menu *submenu;
    struct menuOption *next;
    struct menuOption *prev;

    unsigned char *data;
};