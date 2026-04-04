#include "statem.h"

void *statem_init(statem_t *$, struct statem_entry_t *entries)
{
    $->entries = entries;
    $->count = 0;

    while (entries[$->count].ptr)
        ++$->count;

    return statem_goto($, 0);
}

uint16_t statem_tick(statem_t *$)
{
    return ++$->timer >= $->duration;
}

void *statem_next(statem_t *$)
{
    return statem_goto($, $->index + 1 < $->count ? $->index + 1 : 0);
}

void *statem_prev(statem_t *$)
{
    return statem_goto($, ($->index ?: $->count) - 1);
}

void *statem_goto(statem_t *$, uint16_t index)
{
    if (!$->count || index >= $->count)
        return 0;

    $->ptr = $->entries[index].ptr;
    $->duration = $->entries[index].duration;
    $->index = index;
    $->timer = 0;

    return $->ptr;
}

uint16_t statem_getProgress(statem_t *$)
{
    return $->duration ? ($->timer * 100) / $->duration : 0;
}

void *statem_reset(statem_t *$)
{
    return statem_goto($, 0);
}
