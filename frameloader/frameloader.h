#pragma once

#include <stdint.h>

typedef struct
{
    void (*update)();
    uint16_t vrampos;

    void *resource;
    uint16_t countdown, timer;
    uint16_t frame, num_frames;

    //
    uint32_t external;
} frameloader;

void frameloader_init(frameloader *, void (*)(), uint16_t);
void frameloader_set(frameloader *,  void *, uint16_t, uint16_t);
void frameloader_update(frameloader *);
uint16_t frameloader_isLastFrame(frameloader *);
uint16_t frameloader_isLastTick(frameloader *);
void frameloader_exec(frameloader *);
