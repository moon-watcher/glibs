#include "uclist.h"

int uclist_findIndex(uclist *const this, void *const data)
{
    for (unsigned i = 0; i < this->size; i++)
    {
        if (this->list[i] == data)
        {
            return i;
        }
    }

    return UCLIST_NOT_FOUND;
}

void uclist_removeByIndex(uclist *const this, unsigned index, void (*exec)())
{
    if (this->size == 0)
    {
        return;
    }
    else if (exec != 0)
    {
        exec(this->list[index]);
    }

    --this->size;

    void *const swap = this->list[index];
    this->list[index] = this->list[this->size];
    this->list[this->size] = swap;
}

#define FUNC(NAME, ...)                                                         \
    static void NAME(void *list[], void (*it)(), unsigned size, unsigned items) \
    {                                                                           \
        for (unsigned i = 0; i < size; i += items)                              \
            it(__VA_ARGS__);                                                    \
    }

    FUNC(f1, list[i + 0]);
    FUNC(f2, list[i + 0], list[i + 1]);
    FUNC(f3, list[i + 0], list[i + 1], list[i + 2]);
    FUNC(f4, list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
    FUNC(f5, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);

int uclist_iteratorEx(uclist *const this, void (*iterator)(), unsigned nbItems)
{
    if (nbItems == 0)
    {
        return UCLIST_NO_ITEMS;
    }
    else if (iterator == 0)
    {
        return UCLIST_NULL_ITERATOR;
    }

    static void (*const _exec[])() = {0, f1, f2, f3, f4, f5};

    _exec[nbItems](this->list, iterator, this->size, nbItems);

    return this->size / nbItems;
}
