#pragma once

typedef struct
{
    unsigned max_blocks;
    unsigned chunk_size;
    unsigned next_free;
    unsigned char memory[];
} PoolAllocator;

#define POOLALLOCATOR_SIZE(BLOCKS, SIZE) (sizeof(PoolAllocator) + ((BLOCKS) * (SIZE)))

void PoolAllocator_init(PoolAllocator *const, unsigned, unsigned);
void *PoolAllocator_alloc(PoolAllocator *const);
void PoolAllocator_iterate(PoolAllocator *const, void (*)(void *));
void PoolAllocator_free(PoolAllocator *const, void *);
