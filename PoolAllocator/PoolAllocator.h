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


/*

/// deepseek

#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include <stddef.h>

typedef struct PoolAllocator {
    unsigned max_blocks;
    unsigned chunk_size;
    unsigned next_free;
    unsigned char memory[];   // zona flexible: primero un byte por bloque (flags), luego los bloques de datos
} PoolAllocator;

// Tamaño total: cabecera + flags (max_blocks bytes) + bloques (max_blocks * chunk_size)
#define POOLALLOCATOR_SIZE(BLOCKS, CHUNK_SIZE) \
    (sizeof(PoolAllocator) + (BLOCKS) + ((BLOCKS) * (CHUNK_SIZE)))

void PoolAllocator_init(PoolAllocator *pool, unsigned max_blocks, unsigned chunk_bytes);
void *PoolAllocator_alloc(PoolAllocator *pool);
void PoolAllocator_free(PoolAllocator *pool, void *chunk);
void PoolAllocator_iterate(PoolAllocator *pool, void (*callback)(void *));

#endif

*/
