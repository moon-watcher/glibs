#pragma once

#define BITFLAG_SET(BF, FLAG)   ((*BF) |=  (1U << (FLAG)))
#define BITFLAG_GET(BF, FLAG)   ((*BF) &   (1U << (FLAG)))
#define BITFLAG_CLEAR(BF, FLAG) ((*BF) &= ~(1U << (FLAG)))
#define BITFLAG_FLIP(BF, FLAG)  ((*BF) ^=  (1U << (FLAG)))
