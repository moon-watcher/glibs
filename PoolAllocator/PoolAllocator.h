#pragma once

typedef struct
{
    unsigned short max_blocks;
    unsigned short chunk_size;
    unsigned short next_free;
    unsigned char memory[];
} PoolAllocator;

#define POOLALLOCATOR_SIZE(BLOCKS, SIZE) sizeof(PoolAllocator) + (BLOCKS * SIZE)

void PoolAllocator_init(PoolAllocator *const, unsigned short, unsigned short);
void *PoolAllocator_alloc(PoolAllocator *const);
void PoolAllocator_iterate(PoolAllocator *const, void (*)(void *));
void PoolAllocator_free(PoolAllocator *const, void *);
