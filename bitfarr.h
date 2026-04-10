#pragma once
#include <stdint.h>

#define BITFARR_DECLARE(NAME, SIZE) uint16_t NAME[((SIZE) + 15) >> 4]
#define BITFARR_SET(BF, FLAG)   ((BF)[(FLAG) >> 4] |=  (1U << ((FLAG) & 15)))
#define BITFARR_GET(BF, FLAG)   ((BF)[(FLAG) >> 4] &   (1U << ((FLAG) & 15)))
#define BITFARR_CLEAR(BF, FLAG) ((BF)[(FLAG) >> 4] &= ~(1U << ((FLAG) & 15)))
#define BITFARR_FLIP(BF, FLAG)  ((BF)[(FLAG) >> 4] ^=  (1U << ((FLAG) & 15)))

#include <string.h>
#define BITFARR_ZERO(BF) memset((BF), 0, sizeof(BF))
