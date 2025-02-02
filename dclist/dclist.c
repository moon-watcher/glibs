// Dynamic Cacheable List

#include "dclist.h"
#include "config.h"

void dclist_init(dclist *const this, unsigned maxItemSize)
{
    memset(this, 0, sizeof(dclist));
    this->itemSize = maxItemSize;
    this->ordered = 1;
}

void *dclist_alloc(dclist *const this)
{
    void *ptr = this->list[this->size];

    if (this->size < this->capacity)
    {
        ++this->size;
    }
    else if ((ptr = malloc(this->itemSize)) != 0)
    {
        ptr = dclist_add(this, ptr);
    }

    memset(ptr, 0, this->itemSize);
    return ptr;
}

void *dclist_add(dclist *const this, void *const add)
{
    if (add != 0)
    {
        if (this->size >= this->capacity)
        {
            void *ptr = malloc((this->capacity + 1) * sizeof(void *));

            if (ptr == 0)
            {
                return 0;
            }

            memcpy(ptr, this->list, this->capacity * sizeof(void *));
            free(this->list);
            this->list = ptr;
            ++this->capacity;
        }

        this->list[this->size] = add;
        ++this->size;
    }

    return add;
}

int dclist_iterator(dclist *const this, void (*iterator)())
{
    if (iterator == 0)
    {
        return -1;
    }

    for (unsigned i = 0; i < this->size; ++i)
    {
        iterator(this->list[i]);
    }

    return this->size;
}

int dclist_remove(dclist *const this, void *const data, void *(*exec)())
{
    int index = dclist_findIndex(this, data);

    if (index != DCLIST_NOT_FOUND)
    {
        index = dclist_removeByIndex(this, index, exec);
    }

    return index;
}

void dclist_reset(dclist *const this)
{
    this->size = 0;
}

void dclist_end(dclist *const this)
{
    while (this->itemSize && this->capacity--)
    {
        free(this->list[this->capacity]);
    }

    free(this->list);
    dclist_init(this, this->itemSize);
}

//

int dclist_findIndex(dclist *const this, void *const data)
{
    for (unsigned i = 0; i < this->size; i++)
    {
        if (this->list[i] == data)
        {
            return i;
        }
    }

    return DCLIST_NOT_FOUND;
}

int dclist_removeByIndex(dclist *const this, unsigned index, void (*exec)())
{
    if (this->size == 0 || index >= this->size)
    {
        return DCLIST_NOT_FOUND;
    }
    else if (exec != 0)
    {
        exec(this->list[index]);
    }

    this->size--;

    if (this->ordered)
    {
        for (unsigned i = index; i < this->size; ++i)
        {
            this->list[i] = this->list[i + 1];
        }
    }
    else
    {
        void *const swap = this->list[index];
        this->list[index] = this->list[this->size];
        this->list[this->size] = swap;
    }

    return index;
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

int dclist_iteratorEx(dclist *const this, void (*iterator)(), unsigned nbItems)
{
    if (nbItems == 0)
    {
        return DCLIST_NO_ITEMS;
    }
    else if (iterator == 0)
    {
        return DCLIST_NULL_ITERATOR;
    }

    static void (*const _exec[])() = {0, f1, f2, f3, f4, f5};

    _exec[nbItems](this->list, iterator, this->size, nbItems);

    return this->size / nbItems;
}
