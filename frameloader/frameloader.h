#pragma once

#include <stdint.h>

typedef struct
{
    void (*update_f)();
    uint16_t vrampos;
    void *resource;
    uint16_t countdown;
    int16_t timer;
    // int16_t anim;
    uint16_t frame;
    uint16_t num_frames;
} frameloader;

void frameloader_init(frameloader *, void (*)(), uint16_t);
void frameloader_set(frameloader *,  void *, int16_t, int16_t);
uint16_t frameloader_update(frameloader *);
uint16_t frameloader_isLastFrame(frameloader *);
