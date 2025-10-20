#pragma once

typedef struct
{
    unsigned index;
    unsigned size;
} incremental;

void incremental_init(incremental *const i, unsigned index)
{
    i->index = index;
    i->size = 0;
}

unsigned incremental_get(incremental *const i, unsigned chunk_size)
{
    unsigned index = i->index;

    i->index += chunk_size;
    ++i->size;

    return index;
}
