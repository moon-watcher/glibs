#pragma once

#include <stdint.h>

struct pool
{
    uint8_t *buffer;
    uint16_t size;
    uint16_t offset;
};

void pool_init(struct pool *, void *, uint16_t);
void *pool_alloc(struct pool *, uint16_t, uint16_t);

#define POOL_INIT(POOL, BUFFER) \
    pool_init((POOL), (BUFFER), sizeof(BUFFER))

#define POOL_ALLOC(POOL, TYPE) \
    pool_alloc((POOL), sizeof(TYPE), _Alignof(TYPE))
