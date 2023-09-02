#pragma once

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
void display_on(unsigned int);
void display_off(unsigned int);
void display_prepare(void *const, int, unsigned int);
void display_fade(unsigned int, unsigned int *const);