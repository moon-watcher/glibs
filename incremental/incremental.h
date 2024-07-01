#pragma once

typedef struct
{
    unsigned index;
    unsigned size;
} incremental;

void incremental_init(incremental *const, unsigned);
unsigned incremental_get(incremental *const, unsigned);