#pragma once
#include <stdint.h>

struct statem_entry_t
{
    void *ptr;
    uint32_t duration;
};

typedef struct
{
    struct statem_entry_t *entries;
    struct statem_entry_t; // ptr & duration
    uint16_t index;
    uint32_t timer;
    uint16_t count;
} statem_t;

#define STATEM_TABLE(name, ...) \
    struct statem_entry_t name[] = {__VA_ARGS__}

void *statem_init(statem_t *, struct statem_entry_t *);
uint16_t statem_tick(statem_t *);
void *statem_next(statem_t *);
void *statem_prev(statem_t *);
void *statem_goto(statem_t *, uint16_t);
uint16_t statem_getProgress(statem_t *);
void *statem_reset(statem_t *);



/*
// statem.h
#pragma once
#include <stdint.h>

struct statem_entry_t
{
    void *ptr;
    void *(*condition)(void *);
    uint32_t duration;
};

typedef struct
{
    struct statem_entry_t *entries;
    struct statem_entry_t;
    uint16_t index;
    uint16_t count;
    uint32_t timer;
} statem_t;

#define STATEM_TABLE(name, ...) \
    struct statem_entry_t name[] = {__VA_ARGS__, {0, 0, 0}}

#define STATEM_ENTRY_TIMED(ptr, dur) {ptr, 0, dur}
#define STATEM_ENTRY_COND(ptr, cond) {ptr, cond, 0}
#define STATEM_ENTRY_MIXED(ptr, dur, cond) {ptr, cond, dur}

void *statem_init(statem_t *, struct statem_entry_t *);
uint16_t statem_tick(statem_t *);
void *statem_check(statem_t *, void *);
void *statem_next(statem_t *);
void *statem_prev(statem_t *);
void *statem_goto(statem_t *, uint16_t);
void *statem_reset(statem_t *);
uint16_t statem_getProgress(statem_t *);
*/
