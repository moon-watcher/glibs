#pragma once
#include <stdint.h>

typedef struct
{
    void *ptr;
    uint32_t duration;
} statem_entry_t;

typedef struct
{
    statem_entry_t *entries;
    statem_entry_t *current;
    uint32_t timer;
} statem_t;

#define STATEM_TABLE(name, ...) \
    statem_entry_t name[] = {__VA_ARGS__}

void statem_init(statem_t *, statem_entry_t *);
uint16_t statem_tick(statem_t *);
void *statem_next(statem_t *);
