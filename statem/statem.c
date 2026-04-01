#include "statem.h"

void statem_init(statem_t *$, struct statem_entry_t *entries)
{
    $->entries = entries;
    $->current = entries[0];
    $->index = 0;
    $->timer = 0;
}

uint16_t statem_tick(statem_t *$)
{
    return ++$->timer >= $->current.duration;
}

void *statem_next(statem_t *$)
{
    struct statem_entry_t *next = $->entries + $->index + 1;

    $->index = next->ptr ? ++$->index : 0;
    $->current = $->entries[$->index];
    $->timer = 0;

    return $->current.ptr;
}

void statem_setDuration(statem_t *$, uint32_t duration)
{
    $->current.duration = duration;
}

uint32_t statem_getDuration(statem_t *$)
{
    return $->current.duration;
}

uint16_t statem_getProgress(statem_t *$)
{
    uint32_t d = $->current.duration;
    return d ? ($->timer * 100) / d : 0;
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
