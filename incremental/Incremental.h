#pragma once

typedef struct
{
    unsigned int index;
    unsigned int size;
} Incremental_t;

void incremental_init(Incremental_t *const, unsigned int);
unsigned int incremental_get(Incremental_t *const, unsigned int);