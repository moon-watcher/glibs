#include "statem.h"

void statem_init(statem_t *$, statem_entry_t *entries)
{
    $->entries = $->current = entries;
    $->timer = 0;
}

uint16_t statem_tick(statem_t *$)
{
    return ++$->timer >= $->current->duration;
}

void *statem_next(statem_t *$)
{
    statem_entry_t *next = $->current + 1;

    $->current = next->ptr ? next : $->entries;
    $->timer = 0;

    return $->current->ptr;
}

// void *statem_reset(statem_t *$)
// {
//     $->current = $->entries;
//     $->timer = 0;

//     return $->current->ptr;
// }

// uint16_t statem_getIndex(statem_t *$)
// {
//     return $->current - $->entries;
// }

// uint16_t statem_getProgress(statem_t *$)
// {
//     return $->current->duration ? ($->timer * 100) / $->current->duration: 0;
// }

// void *statem_goto(statem_t *$, uint16_t index)
// {
//     $->current = $->entries + index;
//     $->timer = 0;

//     return $->current->state;
// }

// void *statem_prev(statem_t *$)
// {
//     uint16_t index = $->current == $->entries
//                          ? $->count - 1
//                          : $->current - $->entries - 1;

//     return statem_goto($, index);
// }

// void statem_setDuration(statem_t *$, uint16_t duration)
// {
//     $->current->duration = duration;
// }

// void *statem_getCurrent(statem_t *$)
// {
//     return $->current->state;
// }
