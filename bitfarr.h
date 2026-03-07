#pragma once
#include <stdint.h>

#define CREATE_BITFARR(NAME, SIZE) uint16_t NAME[((SIZE) + 15) >> 4]

#define BITFARR_INDEX(FLAG) ((FLAG) >> 4)
#define BITFARR_BIT(FLAG)   ((FLAG) & 0xF)

inline void     bitfarr_set  (uint16_t *flags, uint16_t flag) {        flags[BITFARR_INDEX(flag)] |=  (1U << BITFARR_BIT(flag)); }
inline uint16_t bitfarr_get  (uint16_t *flags, uint16_t flag) { return flags[BITFARR_INDEX(flag)] &   (1U << BITFARR_BIT(flag)); }
inline void     bitfarr_clear(uint16_t *flags, uint16_t flag) {        flags[BITFARR_INDEX(flag)] &= ~(1U << BITFARR_BIT(flag)); }
inline void     bitfarr_flip (uint16_t *flags, uint16_t flag) {        flags[BITFARR_INDEX(flag)] ^=  (1U << BITFARR_BIT(flag)); }
