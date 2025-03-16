#include "PoolAllocator.h"


void PoolAllocator_init(PoolAllocator *const pool, unsigned max_blocks, unsigned chunk_bytes)
{
    pool->max_blocks = max_blocks;
    pool->chunk_size = chunk_bytes;
    pool->next_free = 0;

    unsigned *mem = (unsigned *)pool->memory;

    for (unsigned i = 0; i < max_blocks - 1; i++)
        mem[i] = i + 1;

    mem[max_blocks - 1] = max_blocks;
}

void *PoolAllocator_alloc(PoolAllocator *const pool)
{
    if (pool->next_free >= pool->max_blocks) return 0;

    unsigned *mem = (unsigned *)pool->memory;
    unsigned index = pool->next_free;
    pool->next_free = mem[index];

    return &mem[index * pool->chunk_size];
}

void PoolAllocator_iterate(PoolAllocator *const pool, void (*callback)(void *))
{
    unsigned *mem = (unsigned *)pool->memory;

    for (unsigned i = 0; i < pool->max_blocks; i++)
        if (mem[i] != pool->next_free)
            callback(&mem[i * pool->chunk_size]);
}

void PoolAllocator_free(PoolAllocator *const pool, void *chunk)
{
    unsigned *mem = (unsigned *)pool->memory;
    unsigned index = ((unsigned *)chunk - mem) / pool->chunk_size;

    mem[index] = pool->next_free;
    pool->next_free = index;
}

/*


void test_PoolAllocator_init()
{
    PoolAllocator pool;
    unsigned short max_blocks = 10;
    unsigned short chunk_size = 32;

    PoolAllocator_init(&pool, max_blocks, chunk_size);

    assert(pool.max_blocks == max_blocks);
    assert(pool.chunk_size == chunk_size);
    assert(pool.next_free == 0);

    unsigned short *mem = (unsigned short *)pool.memory;

    for (unsigned i = 0; i < max_blocks - 1; i++)
    {
        assert(mem[i] == i + 1); // Los bloques deben apuntar al siguiente
    }
    assert(mem[max_blocks - 1] == max_blocks); // El último bloque debe apuntar al fin
}



void test_PoolAllocator_alloc()
{
    PoolAllocator pool;
    unsigned short max_blocks = 5;
    unsigned short chunk_size = 32;

    PoolAllocator_init(&pool, max_blocks, chunk_size);

    void *chunk1 = PoolAllocator_alloc(&pool);
    void *chunk2 = PoolAllocator_alloc(&pool);

    assert(chunk1 != NULL); // Debe asignar un bloque
    assert(chunk2 != NULL); // Debe asignar otro bloque

    unsigned short *mem = (unsigned short *)pool.memory;
    unsigned short *allocated_chunk1 = (unsigned short *)chunk1;
    unsigned short *allocated_chunk2 = (unsigned short *)chunk2;

    // Verifica que los bloques asignados no sean el mismo
    assert(allocated_chunk1 != allocated_chunk2);

    // Verifica que `next_free` fue actualizado correctamente
    assert(pool.next_free == 2); // Después de asignar dos bloques, `next_free` debe ser 2
}




void test_PoolAllocator_iterate()
{
    PoolAllocator pool;
    unsigned short max_blocks = 5;
    unsigned short chunk_size = 32;

    PoolAllocator_init(&pool, max_blocks, chunk_size);

    // Asignamos algunos bloques
    void *chunk1 = PoolAllocator_alloc(&pool);
    void *chunk2 = PoolAllocator_alloc(&pool);

    int count = 0;
    PoolAllocator_iterate(&pool, [](void *chunk) {
        count++;
    });

    assert(count == 2); // Debe iterar sobre los bloques asignados
}




void test_PoolAllocator_free()
{
    PoolAllocator pool;
    unsigned short max_blocks = 5;
    unsigned short chunk_size = 32;

    PoolAllocator_init(&pool, max_blocks, chunk_size);

    // Asignamos y liberamos un bloque
    void *chunk1 = PoolAllocator_alloc(&pool);
    void *chunk2 = PoolAllocator_alloc(&pool);
    PoolAllocator_free(&pool, chunk1);

    assert(pool.next_free == 0); // El primer bloque debe ser liberado, `next_free` debe ser 0

    // Asignamos otro bloque y verificamos si se reutiliza el espacio liberado
    void *chunk3 = PoolAllocator_alloc(&pool);
    assert(chunk3 == chunk1); // `chunk3` debe ser el mismo bloque que `chunk1`, ya que fue liberado
}





void test_PoolAllocator_complete_lifecycle()
{
    PoolAllocator pool;
    unsigned short max_blocks = 5;
    unsigned short chunk_size = 32;

    PoolAllocator_init(&pool, max_blocks, chunk_size);

    // Asignar todos los bloques
    void *chunks[5];
    for (unsigned i = 0; i < max_blocks; i++)
    {
        chunks[i] = PoolAllocator_alloc(&pool);
        assert(chunks[i] != NULL); // Verificar que cada asignación tenga éxito
    }

    // Liberar todos los bloques
    for (unsigned i = 0; i < max_blocks; i++)
    {
        PoolAllocator_free(&pool, chunks[i]);
    }

    // Verificar que los bloques estén nuevamente disponibles
    for (unsigned i = 0; i < max_blocks; i++)
    {
        void *chunk = PoolAllocator_alloc(&pool);
        assert(chunk != NULL); // Verificar que se puede volver a asignar
    }
}





void main()
{
    #include "PoolAllocator.h"

    test_PoolAllocator_init();
    test_PoolAllocator_alloc();
    test_PoolAllocator_iterate();
    test_PoolAllocator_free();
    test_PoolAllocator_complete_lifecycle();

    printf("All tests passed!\n");


    


    void print_pool(PoolAllocator *const pool)
    {
        for (uint16_t i = 0; i < pool->max_blocks; ++i)
        {
            void *chunk = &pool->memory[i * pool->half_chunk_size];
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
