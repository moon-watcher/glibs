#pragma once

#define bitflag_set(flags, flag)   ((*flags) |=  (1U << (flag)))
#define bitflag_get(flags, flag)   ((*flags) &   (1U << (flag)))
#define bitflag_clear(flags, flag) ((*flags) &= ~(1U << (flag)))
#define bitflag_flip(flags, flag)  ((*flags) ^=  (1U << (flag)))
