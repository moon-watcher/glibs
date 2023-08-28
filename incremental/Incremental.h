#pragma once

typedef struct
{
    unsigned int index;
    unsigned int size;
} incremental;

void incremental_init(incremental *const, unsigned int);
unsigned int incremental_get(incremental *const, unsigned int);