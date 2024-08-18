#pragma once

typedef struct
{
    unsigned changed;
    unsigned active;
} joyreader;

void joyreader_init(joyreader *const);
void joyreader_update(joyreader *const, unsigned);
void joyreader_press(joyreader *const, unsigned);

#include "config.h"
