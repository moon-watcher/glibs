#pragma once

// #include <genesis.h>

typedef struct
{
    unsigned int timer;
    unsigned char anim;
    void **animations;
    unsigned char frame;
    unsigned int vram;
} frameloader;

void frameloader_init(frameloader *const, void **, unsigned int, unsigned int (*)(unsigned int));
void frameloader_update(frameloader *const);
void frameloader_setSprite(frameloader *const, void *const);
void frameloader_setAnim(frameloader *const, unsigned int);
void frameloader_reset(frameloader *const);