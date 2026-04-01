#pragma once
#include <stdint.h>

typedef struct
{
    struct statem_entry_t
    {
        void *ptr;
        uint32_t duration;
    } *entries, current;

    uint16_t index;
    uint32_t timer;
} statem_t;

#define STATEM_TABLE(name, ...) \
    struct statem_entry_t name[] = {__VA_ARGS__}

void statem_init(statem_t *, struct statem_entry_t *);
uint16_t statem_tick(statem_t *);
void *statem_next(statem_t *);
void statem_setDuration(statem_t *, uint32_t);
uint32_t statem_getDuration(statem_t *);
uint16_t statem_getProgress(statem_t *);
