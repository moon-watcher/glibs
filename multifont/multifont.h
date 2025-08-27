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
    void *array[80];
    void *definition;
    int total;
    void (*freeFn)();
    multifont *mf;
} multifont_sprite;

void multifont_init(multifont *const, const unsigned long *, unsigned, unsigned, unsigned (*)(unsigned), unsigned, unsigned);
void multifont_text_write(multifont *const, const char *, unsigned, unsigned);
void multifont_reset(multifont *const);

//

multifont_sprite *multifont_sprite_init(multifont *const, void *const, void(*));
void multifont_sprite_write(multifont_sprite *, const char *, unsigned, unsigned);
void multifont_sprite_end(multifont_sprite *); 
