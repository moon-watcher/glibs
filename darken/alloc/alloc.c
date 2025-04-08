// Dynamic Cacheable List

#include "alloc.h"
#include "../config.h"

void de_alloc_init(de_alloc *const this, unsigned maxItemSize)
{
    memset(this, 0, sizeof(de_alloc));
    this->itemSize = maxItemSize;
    // this->ordered = 1;
}

void *de_alloc_alloc(de_alloc *const this)
{
    void *ptr = this->list[this->size];

    if (this->size < this->capacity)
        ++this->size;
    else if ((ptr = malloc(this->itemSize)) != 0)
    {
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
    }

    memset(ptr, 0, this->itemSize);
    return ptr;
}

int de_alloc_iterator(de_alloc *const this, void (*iterator)())
{
    if (iterator == 0) return DE_ALLOC_NULL_ITERATOR;

    for (unsigned i = 0; i < this->size; ++i)
        iterator(this->list[i]);

    return this->size;
}

int de_alloc_remove(de_alloc *const this, void *const data)
{
    int index = de_alloc_findIndex(this, data);

    if (index != DE_ALLOC_NOT_FOUND)
        index = de_alloc_removeByIndex(this, index);

    return index;
}

int de_alloc_restore(de_alloc *const this, void *const data)
{
    for (unsigned i = this->size; i < this->capacity; i++)
        if (this->list[i] == data)
        {
            this->list[i] = this->list[this->size];
            this->list[this->size] = data;
            
            return this->size++;
        }

    return DE_ALLOC_NOT_FOUND;
}

void de_alloc_reset(de_alloc *const this)
{
    this->size = 0;
}

void de_alloc_end(de_alloc *const this)
{
    while (this->itemSize && this->capacity--)
        free(this->list[this->capacity]);

    free(this->list);
    de_alloc_init(this, this->itemSize);
}

//

int de_alloc_findIndex(de_alloc *const this, void *const data)
{
    for (unsigned i = 0; i < this->size; i++)
        if (this->list[i] == data)
            return i;

    return DE_ALLOC_NOT_FOUND;
}

int de_alloc_removeByIndex(de_alloc *const this, unsigned index)
{
    if (this->size == 0 || index >= this->size) return DE_ALLOC_NOT_FOUND;
    
    this->size--;

    // if (this->ordered)
    //     for (unsigned i = index; i < this->size; ++i)
    //         this->list[i] = this->list[i + 1];
    // else
    // {
        void *const swap = this->list[index];
        this->list[index] = this->list[this->size];
        this->list[this->size] = swap;
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

int de_alloc_iteratorEx(de_alloc *const this, void (*iterator)(), unsigned nbItems)
{
    if (nbItems == 0 || nbItems > 5) return DE_ALLOC_NO_ITEMS;
    if (iterator == 0) return DE_ALLOC_NULL_ITERATOR;

    static void (*const _exec[])() = {0, f1, f2, f3, f4, f5};

    _exec[nbItems](this->list, iterator, this->size, nbItems);

    return this->size / nbItems;
}
