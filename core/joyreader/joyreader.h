#pragma once

typedef struct
{
    unsigned port : 2;
    unsigned (*reader_f)(unsigned);
    unsigned changed;
    unsigned active;
} joyreader;

void joyreader_init(joyreader *const, unsigned, unsigned (*)(unsigned));
void joyreader_update(joyreader *const);
void joyreader_reset(joyreader *const);
void joyreader_press(joyreader *const, unsigned);


#include "../../config/joyreader.h"

#include GLIBS_JOYREADER_HELPER_FILE