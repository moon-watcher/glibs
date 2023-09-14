#pragma once

typedef struct
{
    const unsigned long *tiles_ptr;
    const unsigned int *palette_data;
    unsigned int nb_plan;
    unsigned int pal;
    unsigned int prio;
    unsigned int (*vrampos_f)(unsigned int);
    unsigned int chars_number;
    unsigned char pal_counter;
    unsigned char char_width;  // in tiles
    unsigned char char_height; // in tiles
} multifont;

void multifont_init(multifont *const, const unsigned long *, const unsigned int *, unsigned int, unsigned int, unsigned int (*)(unsigned int), unsigned int);

#include "multitext.h"

#include "config/helper.h"
#include GLIBS_MULTIFONT_HELPER_FILE
