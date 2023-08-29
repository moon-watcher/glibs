#pragma once

#include "multifont.h"

typedef struct
{
    multifont *mf;
    unsigned int pos_in_tileset;
    unsigned char width;  // in tiles
    unsigned char height; // in tiles
    unsigned int chars_vrampos[128];
} multifontText;

void multifont_text_init(multifontText *const, multifont *const, unsigned int);
void multifont_text_setWidth(multifontText *const, unsigned int);
void multifont_text_setHeight(multifontText *const, unsigned int);
void multifont_text_writeCharEx(multifontText *const, char, unsigned int, unsigned int, int, int, int);
void multifont_text_writeChar(multifontText *const, char, unsigned int, unsigned int);
void multifont_text_writeEx(multifontText *const, const char *const, unsigned int, unsigned int, int, int, int);
void multifont_text_write(multifontText *const, const char *const, unsigned int, unsigned int);