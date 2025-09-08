#pragma once

void bitflag_init(unsigned *);
void bitflag_add(unsigned *, unsigned);
void bitflag_clear(unsigned *, unsigned);
unsigned bitflag_has(unsigned *, unsigned);
void bitflag_toggle(unsigned *, unsigned);

#define BITFLAG(bit) (1 << (bit))
