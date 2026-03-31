#include "statem.h"

void statem_init(statem_t *sm, statem_entry_t *entries)
{
    sm->entries = sm->current = entries;
    sm->timer = 0;
}

uint16_t statem_tick(statem_t *sm)
{
    return ++sm->timer >= sm->current->duration;
}

void *statem_next(statem_t *sm)
{
    statem_entry_t *next = sm->current + 1;

    sm->current = next->state ? next : sm->entries;
    sm->timer = 0;

    return sm->current->state;
}

// void *statem_reset(statem_t *sm)
// {
//     sm->current = sm->entries;
//     sm->timer = 0;

//     return sm->current->state;
// }

// uint16_t statem_getIndex(statem_t *sm)
// {
//     return sm->current - sm->entries;
// }

// uint16_t statem_getProgress(statem_t *sm)
// {
//     return sm->current->duration ? sm->timer * 100 / sm->current->duration : 100;
// }

// void *statem_goto(statem_t *sm, uint16_t index)
// {
//     sm->current = sm->entries + index;
//     sm->timer = 0;

//     return sm->current->state;
// }

// void *statem_prev(statem_t *sm)
// {
//     uint16_t index = sm->current == sm->entries
//                          ? sm->count - 1
//                          : sm->current - sm->entries - 1;

//     return statem_goto(sm, index);
// }

// void statem_setDuration(statem_t *sm, uint16_t duration)
// {
//     sm->current->duration = duration;
// }

// void *statem_getCurrent(statem_t *sm)
// {
//     return sm->current->state;
// }
