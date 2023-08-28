#include "../incremental/Incremental.h"

static Incremental_t vrampos_incremental;

void vrampos_init()
{
    incremental_init(&vrampos_incremental, 16);
}

unsigned int vrampos_get(unsigned int size)
{
    return incremental_get(&vrampos_incremental, size);
}