#include "pool.h"

void pool_init(struct pool *pool, void *buffer, uint16_t buffer_size)
{
    pool->buffer = (uint8_t *)buffer;
    pool->size = buffer_size;
    pool->offset = 0;
}

void *pool_alloc(struct pool *pool, uint16_t size, uint16_t align)
{
    if (!pool->buffer)
        return 0;

    uint32_t start = (uint32_t)(pool->buffer + pool->offset);
    uint32_t aligned = (start + align - 1) & ~(align - 1);
    uint16_t new_offset = (uint16_t)((aligned - (uint32_t)pool->buffer) + size);

    if (new_offset > pool->size)
        return 0;

    pool->offset = new_offset;

    return (void *)aligned;
}
