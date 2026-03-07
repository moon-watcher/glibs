#pragma once

inline void     bitflag_set  (unsigned *flags, unsigned flag) {        *flags |=  (1U << flag); }
inline unsigned bitflag_get  (unsigned *flags, unsigned flag) { return *flags &   (1U << flag); }
inline void     bitflag_clear(unsigned *flags, unsigned flag) {        *flags &= ~(1U << flag); }
inline void     bitflag_flip (unsigned *flags, unsigned flag) {        *flags ^=  (1U << flag); }
