#pragma once

typedef struct
{
    unsigned timer;
    unsigned char anim;
    void **animations;
    unsigned char frame;
    unsigned vram;
} frameloader;

void frameloader_init(frameloader *const, void **, unsigned, unsigned (*)(unsigned));
void frameloader_update(frameloader *const);
void frameloader_setSprite(frameloader *const, void *const);
void frameloader_setAnim(frameloader *const, unsigned);
void frameloader_reset(frameloader *const);


#include "config.h"
#include FRAMELOADER_HELPER_FILE