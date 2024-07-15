#pragma once

typedef struct
{
    unsigned timer;
    unsigned char anim;
    void **animations;
    unsigned char frame;
    unsigned vram;
} frameloader;

void frameloader_init(frameloader *const, void **, unsigned);
void frameloader_update(frameloader *const);
void frameloader_sprite(frameloader *const, void *const);
void frameloader_anim(frameloader *const, unsigned);
void frameloader_reset(frameloader *const);
