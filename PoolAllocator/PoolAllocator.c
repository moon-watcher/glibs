#include "PoolAllocator.h"

void PoolAllocator_init(PoolAllocator *const pool, unsigned short max_blocks, unsigned short chunk_size)
{
    pool->max_blocks = max_blocks;
    pool->chunk_size = chunk_size;
    pool->next_free = 0;

    for (unsigned short i = 0; i < max_blocks - 1; i++)
        *(unsigned short *)&pool->memory[i * chunk_size] = i + 1;

    *(unsigned short *)&pool->memory[(max_blocks - 1) * chunk_size] = max_blocks;
}

void *PoolAllocator_alloc(PoolAllocator *const pool)
{
    if (pool->next_free >= pool->max_blocks) return 0;

    void *chunk = &pool->memory[pool->next_free * pool->chunk_size];
    pool->next_free = *(unsigned short *)chunk;
    return chunk;
}

void PoolAllocator_iterate(PoolAllocator *const pool, void (*callback)(void *))
{
    unsigned char *ptr = pool->memory;

    for (unsigned short i = 0; i < pool->max_blocks; i++)
    {
        void *chunk = &ptr[i * pool->chunk_size];
        
        if (*(unsigned short *)chunk != pool->next_free)
            callback(chunk);
    }
}

void PoolAllocator_free(PoolAllocator *const pool, void *chunk)
{
    unsigned short index = (unsigned short)(((unsigned char *)chunk - pool->memory) / pool->chunk_size);
    *(unsigned short *)chunk = pool->next_free;
    pool->next_free = index;
}

/*
void main()
{
    #include "PoolAllocator.h"

    void print_pool(PoolAllocator *const pool)
    {
        for (uint16_t i = 0; i < pool->max_blocks; ++i)
        {
            void *chunk = &pool->memory[i * pool->chunk_size];
            kprintf("%02d: %-8p [%s]", i, chunk, (i == pool->next_free) ? "FREE " : "IN USE");
        }
        kprintf("----------------");
    }

    uint16_t max_blocks = 3;
    uint16_t chunk_size = 32;

    PoolAllocator* pool = malloc(POOLALLOCATOR_SIZE(max_blocks, chunk_size));

    void *chunk0, *chunk1, *chunk2, *chunk3, *chunk4;

    kprintf("INIT ---------");
    PoolAllocator_init(pool, max_blocks, chunk_size);
    print_pool(pool); kprintf("   FREE --> %d", pool->next_free);

    kprintf("ADD CHUNCK0 ---------");
    chunk0 = PoolAllocator_alloc(pool);
    kprintf(" --> %p %p %p %p %p", chunk0, chunk1, chunk2, chunk3, chunk4);
    kprintf("   FREE --> %d", pool->next_free);

    kprintf("FREE CHUNK0 ---------");
    PoolAllocator_free(pool, chunk0);
    kprintf(" --> %p %p %p %p %p", chunk0, chunk1, chunk2, chunk3, chunk4);
    kprintf("   FREE --> %d", pool->next_free);


    kprintf("ADD CHUNK 1-4 ---------");
    chunk1 = PoolAllocator_alloc(pool);
    chunk2 = PoolAllocator_alloc(pool);
    chunk3 = PoolAllocator_alloc(pool);
    chunk4 = PoolAllocator_alloc(pool);
    kprintf(" --> %p %p %p %p %p", chunk0, chunk1, chunk2, chunk3, chunk4);
    kprintf("   FREE --> %d", pool->next_free);


    kprintf("FREE CHUNK4 ---------");
    PoolAllocator_free(pool, chunk4);
    kprintf(" --> %p %p %p %p %p", chunk0, chunk1, chunk2, chunk3, chunk4);
    kprintf("   FREE --> %d", pool->next_free);

    kprintf("FREE CHUNK2 ---------");
    PoolAllocator_free(pool, chunk2);
    kprintf(" --> %p %p %p %p %p", chunk0, chunk1, chunk2, chunk3, chunk4);
    kprintf("   FREE --> %d", pool->next_free);

    // pool_free(pool, chunk0);
    // pool_free(pool, chunk1);
    // kprintf(" --> %p %p %p %p %p", chunk0, chunk1, chunk2, chunk3, chunk4);

    // chunk0 = pool_alloc(pool);
    // kprintf(" --> %p %p %p %p %p", chunk0, chunk1, chunk2, chunk3, chunk4);



    // void* chunk3 = pool_alloc(pool);
    // void* chunk4 = pool_alloc(pool);
    // void* chunk5 = pool_alloc(pool);
    // void* chunk6 = pool_alloc(pool);
    // void* chunk7 = pool_alloc(pool);

    // print_pool(pool);
    // kprintf(" - Chunk 3: %p", chunk3);
    // kprintf(" - Chunk 4: %p", chunk4);
    // kprintf(" - Chunk 5: %p", chunk5);
    // kprintf(" - Chunk 6: %p", chunk6);
    // kprintf(" - Chunk 7: %p", chunk7);

    // pool_free(pool, chunk6);
    // print_pool(pool);

    // free(pool);

    return 0;
}
*/
