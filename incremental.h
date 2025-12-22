#pragma once

#include <stdint.h>

typedef struct
{
    uint16_t index;
    uint16_t size;
} incremental;

void incremental_init(incremental *$)
{
    $->index = 0;
    $->size = 0;
}

uint16_t incremental_get(incremental *$, uint16_t chunk_size)
{
    uint16_t index = $->index;

    $->index += chunk_size;
    ++$->size;

    return index;
}
