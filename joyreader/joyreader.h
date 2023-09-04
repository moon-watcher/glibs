#pragma once

typedef struct
{
    unsigned int port : 2;
    unsigned int (*reader_f)(unsigned int);
    unsigned int changed;
    unsigned int active;
} joyreader;

void joyreader_init(joyreader *const, unsigned int, unsigned int (*)(unsigned int));
void joyreader_update(joyreader *const);
void joyreader_reset(joyreader *const);
void joyreader_press(joyreader *const, unsigned int);

#include "helper.h"