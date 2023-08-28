#pragma once

#include <genesis.h>

typedef struct
{
    const Image *image;
    unsigned int plan;
    unsigned int pal;
    unsigned int prio;
    unsigned int (*vramPos_f)(unsigned int);
} Multifont;

void multifont_init(Multifont *const mf, const Image *image, unsigned int plan, unsigned int pal, unsigned int (*vramPos_f)(unsigned int));

typedef struct
{
    Multifont *mf;
    unsigned int pos_in_tileset;
    unsigned char width;  // in tiles
    unsigned char height; // in tiles
    unsigned int chars_vrampos[128];
} MultifontText;

void multifont_text_init(MultifontText *const, Multifont *const, unsigned int);
void multifont_text_setWidth(MultifontText *const, unsigned int);
void multifont_text_setHeight(MultifontText *const, unsigned int);
void multifont_text_writeCharEx(MultifontText *const, char, unsigned int, unsigned int, int, int, int);
void multifont_text_writeChar(MultifontText *const, char, unsigned int, unsigned int);
void multifont_text_writeEx(MultifontText *const, const char *const, unsigned int, unsigned int, int, int, int);
void multifont_text_write(MultifontText *const, const char *const, unsigned int, unsigned int);