#pragma once

#include <stdint.h>

inline uint32_t bitflag_set(uint32_t *obs, uint32_t flag)
{
    return *obs |= (1U << flag);
}

inline uint32_t bitflag_get(uint32_t *obs, uint32_t flag)
{
    return *obs & (1U << flag);
}

inline uint32_t bitflag_clear(uint32_t *obs, uint32_t flag)
{
    return *obs &= ~(1U << flag);
}

inline uint32_t bitflag_flip(uint32_t *obs, uint32_t flag)
{
    return *obs ^= (1U << flag);
}

inline uint32_t bitflag_mask(uint32_t flag)
{
    return (1U << flag);
}


/*
uint32_t flags;

enum { FLAG_1, FLAG_2, FLAG_3, };

kprintf("bitflag_set FLAG_1 // 1 // %d", bitflag_set(&flags, FLAG_1));
// kprintf("bitflag_set FLAG_2 // 2 // %d", bitflag_set(&flags, FLAG_2));
kprintf("bitflag_set FLAG_3 // 4 // %d", bitflag_set(&flags, FLAG_3));

kprintf("bitflag_mask FLAG_1 // 1 // %d", bitflag_mask(FLAG_1));
kprintf("bitflag_mask FLAG_2 // 2 // %d", bitflag_mask(FLAG_2));
kprintf("bitflag_mask FLAG_3 // 4 // %d", bitflag_mask(FLAG_3));

kprintf("%d - %d %d %d ", flags, bitflag_get(&flags, FLAG_1), bitflag_get(&flags, FLAG_2), bitflag_get(&flags, FLAG_3));
*/
