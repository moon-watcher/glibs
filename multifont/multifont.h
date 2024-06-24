#pragma once

#include "config.h"

typedef struct
{
    unsigned long *tiles_ptr;
    unsigned plan;
    unsigned pal;
    unsigned prio;
    unsigned (*vrampos_f)(unsigned);
    unsigned char char_width;  // in tiles
    unsigned char char_height; // in tiles
    unsigned pos_in_tileset;
    unsigned chars_number;
    unsigned chars_vrampos[MULTIFONT_MAXCHARS];
} multifont;

void multifont_init(multifont *const, const unsigned long *, unsigned, unsigned, unsigned (*)(unsigned), unsigned, unsigned);
void multifont_reset(multifont *const);
void multifont_write(multifont *const, char *, unsigned, unsigned);