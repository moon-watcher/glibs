#pragma once

inline void bitflag_set(unsigned *obs, unsigned flag)
{
    *obs |= (1U << flag);
}

inline unsigned bitflag_get(unsigned *obs, unsigned flag)
{
    return *obs & (1U << flag);
}

inline void bitflag_clear(unsigned *obs, unsigned flag)
{
    *obs &= ~(1U << flag);
}

inline void bitflag_flip(unsigned *obs, unsigned flag)
{
    *obs ^= (1U << flag);
}
