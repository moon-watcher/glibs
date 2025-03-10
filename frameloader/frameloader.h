#pragma once

typedef struct
{
    unsigned timer;
    void *resource;
    unsigned char anim;
    unsigned char frame;
    unsigned vrampos;
    unsigned pause;
} frameloader;

void frameloader_init(frameloader *const, void *const, unsigned);
void frameloader_resource(frameloader *const, void *const);
void frameloader_update(frameloader *const);
void frameloader_setSprite(frameloader *const, void *const);
void frameloader_setAnim(frameloader *const, unsigned);
void frameloader_pause(frameloader *const);
void frameloader_resume(frameloader *const);
