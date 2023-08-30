#include "../incremental/incremental.h"

static incremental vraminc_incremental;

void vraminc_init()
{
    incremental_init(&vraminc_incremental, 16);
}

unsigned int vraminc_get(unsigned int size)
{
    return incremental_get(&vraminc_incremental, size);
}