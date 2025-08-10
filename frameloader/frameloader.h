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
    unsigned pause;
} frameloader;

enum
{
    FRAMELOADER_ERROR,
    FRAMELOADER_PAUSED,
    FRAMELOADER_IDLE,
    FRAMELOADER_OK
};

void frameloader_init(frameloader *const, void (*update_f)(struct frameloader *const), unsigned);
void frameloader_set(frameloader *const, const void *, int, int);
unsigned frameloader_update(frameloader *const);
void frameloader_pause(frameloader *const);
void frameloader_resume(frameloader *const);
