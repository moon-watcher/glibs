// Dynamic Cacheable List

#include "list.h"
#include "../config.h"

void de_list_init(de_list *const this)
{
    memset(this, 0, sizeof(de_list));
    // this->ordered = 1;
}

void *de_list_add(de_list *const this, void *const ptr)
{
    if (!ptr) return 0;

    if (this->size >= this->capacity)
    {
        void *aux = malloc((this->capacity + 1) * sizeof(void *));
        if (aux == 0) return 0;

        memcpy(aux, this->items, this->capacity * sizeof(void *));
        free(this->items);
        this->items = aux;
        ++this->capacity;
    }

    this->items[this->size] = ptr;
    ++this->size;

    return ptr;
}

int de_list_iterator(de_list *const this, void (*iterator)())
{
    if (iterator == 0) return DE_LIST_NULL_ITERATOR;

    for (unsigned i = 0; i < this->size; ++i)
        iterator(this->items[i]);

    return this->size;
}

int de_list_remove(de_list *const this, void *const data)
{
    int index = de_list_findIndex(this, data);

    if (index != DE_LIST_NOT_FOUND)
        index = de_list_removeByIndex(this, index);

    return index;
}

void de_list_reset(de_list *const this)
{
    this->size = 0;
}

void de_list_end(de_list *const this)
{
    free(this->items);
    de_list_init(this);
}

//

int de_list_findIndex(de_list *const this, void *const data)
{
    for (unsigned i = 0; i < this->size; i++)
        if (this->items[i] == data)
            return i;

    return DE_LIST_NOT_FOUND;
}

int de_list_removeByIndex(de_list *const this, unsigned index)
{
    if (this->size == 0 || index >= this->size) return DE_LIST_NOT_FOUND;

    this->size--;

    // if (this->ordered)
    //     for (unsigned i = index; i < this->size; ++i)
    //         this->items[i] = this->items[i + 1];
    // else
    // {
        void *const swap = this->items[index];
        this->items[index] = this->items[this->size];
        this->items[this->size] = swap;
    // }

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
FUNC(f6, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5]);

void de_list_iteratorEx(de_list *const this, void (*iterator)(), unsigned nbItems)
{
    if (nbItems == 0 || nbItems > 6) return DE_LIST_NO_ITEMS;
    if (iterator == 0) return DE_LIST_NULL_ITERATOR;

    static void (*const _exec[])() = {0, f1, f2, f3, f4, f5, f6};

    _exec[nbItems](this->items, iterator, this->size, nbItems);
}
