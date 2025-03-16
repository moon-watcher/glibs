// Dynamic Cacheable List

#include "dcalloc.h"
#include "config.h"

void dcalloc_init(dcalloc *const this, unsigned maxItemSize)
{
    memset(this, 0, sizeof(dcalloc));
    this->itemSize = maxItemSize;
    // this->ordered = 1;
}

void *dcalloc_alloc(dcalloc *const this)
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

int dcalloc_iterator(dcalloc *const this, void (*iterator)())
{
    if (iterator == 0) return DCALLOC_NULL_ITERATOR;

    for (unsigned i = 0; i < this->size; ++i)
        iterator(this->list[i]);

    return this->size;
}

int dcalloc_remove(dcalloc *const this, void *const data)
{
    int index = dcalloc_findIndex(this, data);

    if (index != DCALLOC_NOT_FOUND)
        index = dcalloc_removeByIndex(this, index);

    return index;
}

int dcalloc_restore(dcalloc *const this, void *const data)
{
    for (unsigned i = this->size; i < this->capacity; i++)
        if (this->list[i] == data)
        {
            this->list[i] = this->list[this->size];
            this->list[this->size] = data;
            
            return this->size++;
        }

    return DCALLOC_NOT_FOUND;
}

void dcalloc_reset(dcalloc *const this)
{
    this->size = 0;
}

void dcalloc_end(dcalloc *const this)
{
    while (this->itemSize && this->capacity--)
        free(this->list[this->capacity]);

    free(this->list);
    dcalloc_init(this, this->itemSize);
}

//

int dcalloc_findIndex(dcalloc *const this, void *const data)
{
    for (unsigned i = 0; i < this->size; i++)
        if (this->list[i] == data)
            return i;

    return DCALLOC_NOT_FOUND;
}

int dcalloc_removeByIndex(dcalloc *const this, unsigned index)
{
    if (this->size == 0 || index >= this->size) return DCALLOC_NOT_FOUND;
    
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

int dcalloc_iteratorEx(dcalloc *const this, void (*iterator)(), unsigned nbItems)
{
    if (nbItems == 0 || nbItems > 5) return DCALLOC_NO_ITEMS;
    if (iterator == 0) return DCALLOC_NULL_ITERATOR;

    static void (*const _exec[])() = {0, f1, f2, f3, f4, f5};

    _exec[nbItems](this->list, iterator, this->size, nbItems);

    return this->size / nbItems;
}
