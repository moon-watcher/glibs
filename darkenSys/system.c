#include "system.h"
#include "assert.h"

void de_system_init(de_system *const this)
{
    uclist_init(this, 0);
}

void *de_system_add(de_system *const this, void *const data)
{
    void *ret = uclist_add(this, data);
    return DARKEN_ASSERT(ret == 0, ret, "Allocation");
}

int de_system_delete(de_system *const this, void *const data)
{
    int ret = uclist_remove(this, data, 0);
    return DARKEN_ASSERT(ret == UCLIST_NOT_FOUND, ret, "Not found");
}

int de_system_update(de_system *const this, void (*update)(), unsigned params)
{
    DARKEN_ASSERT(update == 0, UCLIST_NULL_ITERATOR, "Null iterator");
    DARKEN_ASSERT(params == 0, UCLIST_NO_ITEMS, "No parameters");

    for (unsigned i = 0; i < this->size; i += params)
    {
        update(this->list[i + 0], this->list[i + 1], this->list[i + 2], this->list[i + 3]);
    }

    return this->size / params;
}

void de_system_reset(de_system *const this)
{
    uclist_reset(this);
}

void de_system_end(de_system *const this)
{
    uclist_end(this);
}
