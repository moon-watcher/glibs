#pragma once

#define BITFLAG_SET(obs, flag) ((obs) |= (1U << (flag)))
#define BITFLAG_TEST(obs, flag) (((obs) & (1U << (flag))) != 0U)
#define BITFLAG_CLEAR(obs, flag) ((obs) &= ~(1U << (flag)))
#define BITFLAG_FLIP(obs, flag) ((obs) ^= (1U << (flag)))
