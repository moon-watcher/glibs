#pragma once

#include "multifont.h"
#include "config/multitext.h"

typedef struct
{
    multifont *mf;
    unsigned int pos_in_tileset;
    unsigned int chars_number;
    unsigned int chars_vrampos[MULTITEXT_MAX_CHARS];
} multitext;

void multitext_init(multitext *const, multifont *const, unsigned int);
void multitext_reset(multitext *const);
void multitext_writeEx(multitext *const, char *const, unsigned int, unsigned int, int, int, int);
void multitext_write(multitext *const, char *const, unsigned int, unsigned int);