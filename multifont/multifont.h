#pragma once

#include "config.h"

typedef struct
{
    const unsigned long *tiles_ptr;
    unsigned plan;
    unsigned pal;
    unsigned prio;
    unsigned (*vrampos_f)(unsigned);
    unsigned char char_width;  // in tiles
    unsigned char char_height; // in tiles
} multifont;

typedef struct
{
    multifont *mf;
    unsigned pos_in_tileset;
    unsigned chars_number;
    unsigned chars_vrampos[MULTIFONT_MAXCHARS];
} multifont_text;

void multifont_init(multifont *const, const unsigned long *, unsigned, unsigned, unsigned (*)(unsigned));
void multifont_text_prepare(multifont_text *const, multifont *const, unsigned, unsigned);
void multifont_text_reset(multifont_text *const);
void multifont_text_writeEx(multifont_text *const, char *, unsigned, unsigned, int, int, int);
void multifont_text_write(multifont_text *const, char *, unsigned, unsigned);