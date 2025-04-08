#include "system.h"
#include "assert.h"

void darkenSys_init(darkenSys *const this)
{
    dclist_init(this);
}

void *darkenSys_add(darkenSys *const this, void *const data)
{
    void *ret = dclist_add(this, data);
    return DARKEN_ASSERT(ret == 0, ret, "Allocation");
}

int darkenSys_delete(darkenSys *const this, void *const data)
{
    int ret = dclist_remove(this, data);
    return DARKEN_ASSERT(ret == DCLIST_NOT_FOUND, ret, "Not found");
}

int darkenSys_update(darkenSys *const this, void (*update)(), unsigned params)
{
    DARKEN_ASSERT(update == 0, DCLIST_NULL_ITERATOR, "Null iterator");
    DARKEN_ASSERT(params == 0, DCLIST_NO_ITEMS, "No parameters");

    for (unsigned i = 0; i < this->size; i += params)
    {
        update(this->list[i + 0], this->list[i + 1], this->list[i + 2], this->list[i + 3]);
    }

    return this->size / params;
}

void darkenSys_reset(darkenSys *const this)
{
    dclist_reset(this);
}

void darkenSys_end(darkenSys *const this)
{
    dclist_end(this);
}
