#pragma once

#include "multifont.h"
#include "config.h"

typedef struct
{
    multifont *mf;
    unsigned int pos_in_tileset;
    unsigned char char_width;  // in tiles
    unsigned char char_height; // in tiles
    unsigned int chars_vrampos[MULTIFONTTEXT_FONT_CHARS];
} multifontText;

void multifont_text_init(multifontText *const, multifont *const, unsigned int);
void multifont_text_writeCharEx(multifontText *const, char, unsigned int, unsigned int, int, int, int);
void multifont_text_writeChar(multifontText *const, char, unsigned int, unsigned int);
void multifont_text_writeEx(multifontText *const, const char *const, unsigned int, unsigned int, int, int, int);
void multifont_text_write(multifontText *const, const char *const, unsigned int, unsigned int);