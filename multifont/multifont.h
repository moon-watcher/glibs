#pragma once

#include <genesis.h>

typedef struct
{
    const Image *image;
    unsigned int plan;
    unsigned int pal;
    unsigned int prio;
    unsigned int (*vramPos_f)(unsigned int);
} multifont;

void multifont_init(multifont *const mf, const Image *image, unsigned int plan, unsigned int pal, unsigned int (*vramPos_f)(unsigned int));

#include "multifontText.h"