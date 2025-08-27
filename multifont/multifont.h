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

typedef struct
{
    void *array[MULTIFONT_SPRITE_MAXSPRITES];
    void *definition;
    int total;
    void (*freeFn)();
    multifont *mf;
    unsigned flags;
} multifont_sprite;

void multifont_text_init(multifont *const, const unsigned long *, unsigned, unsigned, unsigned (*)(unsigned), unsigned, unsigned);
void multifont_text_write(multifont *const, const char *, unsigned, unsigned);
void multifont_text_reset(multifont *const);

//

void multifont_sprite_init(multifont_sprite *const, multifont *const, void *const, void(*));
void multifont_sprite_write(multifont_sprite *const, const char *, unsigned, unsigned);
void multifont_sprite_reset(multifont_sprite *const);
