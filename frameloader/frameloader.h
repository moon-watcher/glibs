#pragma once

#include <stdint.h>

typedef struct
{
    void (*update_f)();
    void *resource;
    uint16_t vrampos;
    uint16_t countdown, timer;
    uint16_t frame, num_frames;
} frameloader;

void frameloader_init(frameloader *, void (*)(), uint16_t);
void frameloader_set(frameloader *,  void *, uint16_t, uint16_t);
void frameloader_update(frameloader *);
uint16_t frameloader_isLastFrame(frameloader *);
uint16_t frameloader_isLastTick(frameloader *);
