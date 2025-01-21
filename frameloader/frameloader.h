#pragma once

typedef struct
{
    unsigned timer;
    void *definition;
    unsigned char anim;
    unsigned char frame;
    unsigned vrampos;
} frameloader;

void frameloader_init(frameloader *const, void *const, unsigned);
void frameloader_update(frameloader *const);
void frameloader_setSprite(frameloader *const, void *const);
void frameloader_setAnim(frameloader *const, unsigned);
