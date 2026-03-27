#pragma once

#include <stdint.h>

typedef struct
{
    void (*update)();
    uint16_t vrampos;

    void *resource;
    uint16_t countdown, timer;
    uint16_t frame, num_frames;
    int16_t loop;

    //
    uint32_t external;
} frameloader;

void frameloader_init(frameloader *, void (*)(), uint16_t);
void frameloader_set(frameloader *,  void *, uint16_t, uint16_t);
void frameloader_setResource(frameloader *, void *, uint16_t);
void frameloader_setTimer(frameloader *, uint16_t);
void frameloader_setLoop(frameloader *, int16_t);
void frameloader_update(frameloader *);
uint16_t frameloader_isLastFrame(frameloader *);
uint16_t frameloader_isLastTick(frameloader *);
int16_t frameloader_getLoop(frameloader *);
void frameloader_exec(frameloader *);
