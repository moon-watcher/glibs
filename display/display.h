#pragma once

#include <genesis.h>
#include "../multifont/multifont.h"

enum
{
    DISPLAY_COLORS,
    DISPLAY_MULTIFONT,
    DISPLAY_SPRITEDEFINITION,
    DISPLAY_PALETTE,
    DISPLAY_SPRITE,
    DISPLAY_IMAGE,
};

void display_init();
void display_on(u16);
void display_off(u16);
void display_prepare(void *, s16, u16);