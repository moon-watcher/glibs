#pragma once

#include <genesis.h>

typedef struct
{
    unsigned int timer;
    unsigned char anim;
    Animation **animations;
    unsigned char frame;
    unsigned int vram;
} frameloader;

void frameloader_init(frameloader *const, const SpriteDefinition *, unsigned int (*)(unsigned int));
void frameloader_update(frameloader *const);
void frameloader_setSprite(frameloader *const, Sprite *const);
void frameloader_setAnim(frameloader *const, unsigned int);
void frameloader_reset(frameloader *const);