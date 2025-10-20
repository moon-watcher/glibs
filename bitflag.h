#pragma once

#define BITFLAG_INIT(obs)          ((obs) = 0u)
#define BITFLAG_ADD(obs, flag)     ((obs) |= (flag))
#define BITFLAG_CLEAR(obs, flag)   ((obs) &= ~(flag))
#define BITFLAG_HAS(obs, flag)    (((obs) & (flag)) != 0u)
#define BITFLAG_TOGGLE(obs, flag)  ((obs) ^= (flag))

#define BITFLAG(bit) (1 << (bit))
