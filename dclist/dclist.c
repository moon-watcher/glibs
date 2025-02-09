// Dynamic Cacheable List

#include "dclist.h"
#include "config.h"

void dclist_init(dclist *const this)
{
    memset(this, 0, sizeof(dclist));
}

void *dclist_add(dclist *const this, void *const ptr)
{
    if (!ptr) return 0;

    if (this->size >= this->capacity)
    {
        void *aux = malloc((this->capacity + 1) * sizeof(void *));
        if (aux == 0) return 0;

        memcpy(aux, this->list, this->capacity * sizeof(void *));
        free(this->list);
        this->list = aux;
        ++this->capacity;
    }

    this->list[this->size] = ptr;
    ++this->size;

    return ptr;
}

int dclist_iterator(dclist *const this, void (*iterator)())
{
    if (iterator == 0) return DCLIST_NULL_ITERATOR;

    for (unsigned i = 0; i < this->size; ++i)
        iterator(this->list[i]);

    return this->size;
}

int dclist_remove(dclist *const this, void *const data)
{
    int index = dclist_findIndex(this, data);

    if (index != DCLIST_NOT_FOUND)
        index = dclist_removeByIndex(this, index);

    return index;
}

void dclist_reset(dclist *const this)
{
    this->size = 0;
}

void dclist_end(dclist *const this)
{
    free(this->list);
    dclist_init(this);
}

//

int dclist_findIndex(dclist *const this, void *const data)
{
    for (unsigned i = 0; i < this->size; i++)
        if (this->list[i] == data)
            return i;

    return DCLIST_NOT_FOUND;
}

int dclist_removeByIndex(dclist *const this, unsigned index)
{
    if (this->size == 0 || index >= this->size) return DCLIST_NOT_FOUND;

    this->size--;

    for (unsigned i = index; i < this->size; ++i)
        this->list[i] = this->list[i + 1];

    // // Unordered delete
    // void *const swap = this->list[index];
    // this->list[index] = this->list[this->size];
    // this->list[this->size] = swap;

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
    if (nbItems == 0 || nbItems > 5) return DCLIST_NO_ITEMS;
    if (iterator == 0) return DCLIST_NULL_ITERATOR;

    static void (*const _exec[])() = {0, f1, f2, f3, f4, f5};

    _exec[nbItems](this->list, iterator, this->size, nbItems);

    return this->size / nbItems;
}
