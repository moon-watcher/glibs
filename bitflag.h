#pragma once

inline void     bitflag_init   (unsigned *obs)                { *obs = 0; }
inline void     bitflag_add    (unsigned *obs, unsigned flag) { *obs |= flag; }
inline void     bitflag_clear  (unsigned *obs, unsigned flag) { *obs &= ~flag; }
inline unsigned bitflag_has    (unsigned *obs, unsigned flag) { return (*obs & flag) != 0; }
inline void     bitflag_toggle (unsigned *obs, unsigned flag) { *obs ^= flag; }

#define BITFLAG(bit) (1 << (bit))
