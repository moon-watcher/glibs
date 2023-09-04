#pragma once

#include "multifont.h"
#include "config.h"

typedef struct
{
    multifont *mf;
    unsigned int pos_in_tileset;
    unsigned int chars_vrampos[MULTIFONT_MAX_CHARS];
} multitext;

void multitext_init(multitext *const, multifont *const);
void multitext_reset(multitext *const);
void multitext_charEx(multitext *const, char, unsigned int, unsigned int, int, int, int);
void multitext_char(multitext *const, char, unsigned int, unsigned int);
void multitext_writeEx(multitext *const, const char *const, unsigned int, unsigned int, int, int, int);
void multitext_write(multitext *const, const char *const, unsigned int, unsigned int);