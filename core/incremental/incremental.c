#include "incremental.h"

void incremental_init(incremental *const i, unsigned int index)
{
    i->index = index;
    i->size = 0;
}

unsigned int incremental_get(incremental *const i, unsigned int chunk_size)
{
    unsigned int index = i->index;

    i->index += chunk_size;
    ++i->size;

    return index;
}
