#pragma once

#include <genesis.h>

typedef struct
{
    const Image *image;
    unsigned int plan;
    unsigned int pal;
    unsigned int prio;
    unsigned int (*vrampos_f)(unsigned int);
} multifont;

void multifont_init(multifont *const, const Image *, unsigned int, unsigned int, unsigned int (*)(unsigned int));

#include "multifontText.h"