#pragma once

typedef struct frameloader
{
    void (*update_f)(struct frameloader *const);
    unsigned vrampos;
    void *resource;
    unsigned countdown;
    int timer;
    int anim;
    unsigned frame;
} frameloader;

void frameloader_init(frameloader *const, void (*)(struct frameloader *const), unsigned);
void frameloader_set(frameloader *const, const void *, int, int);
unsigned frameloader_update(frameloader *const);
