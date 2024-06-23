#pragma once

#include "config.h"
#include MULTIFONT_HELPER

typedef struct
{
    const unsigned long *tiles_ptr;
    const unsigned *palette_data;
    unsigned nb_plan;
    unsigned pal;
    unsigned prio;
    unsigned (*vrampos_f)(unsigned int);
    unsigned chars_number;
    unsigned char pal_counter;
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

void multifont_init(multifont *const, const unsigned long *, const unsigned *, unsigned, unsigned, unsigned (*)(unsigned int), unsigned);
void multifont_text_prepare(multifont_text *const, multifont *const, unsigned);
void multifont_text_reset(multifont_text *const);
void multifont_text_writeEx(multifont_text *const, char *, unsigned, unsigned, int, int, int);
void multifont_text_write(multifont_text *const, char *, unsigned, unsigned);

