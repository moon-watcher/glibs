// Unordered Cacheable List

#include "uclist.h"
#include "config.h"

void uclist_init_alloc(uclist *const this, unsigned short itemSize)
{
    this->items = 0;
    this->size = 0;
    this->capacity = 0;
    this->itemSize = itemSize;
}

void uclist_init_add(uclist *const this)
{
    uclist_init_alloc(this, 0);
}

void *uclist_alloc(uclist *const this)
{
    void *ptr = this->items[this->size];
    unsigned short const itemSize = this->itemSize;

    if (this->size < this->capacity)
        ++this->size;

    else if (!((ptr = malloc(itemSize)) && uclist_add(this, ptr)))
        free(ptr);

    memset(ptr, 0, itemSize);

    return ptr;
}

void *uclist_add(uclist *const this, void *const add)
{
    unsigned short const capacity = this->capacity;

    if (this->size >= capacity)
    {
        void **ptr = malloc((capacity + 1) * sizeof(void *));

        if (!ptr)
            return 0;

        memcpy(ptr, this->items, capacity * sizeof(void *));
        free(this->items);

        this->items = ptr;
        ++this->capacity;
    }

    return this->items[this->size++] = add;
}

void uclist_iterator(uclist *const this, void (*iterator)())
{
    void **const items = this->items;
    unsigned short i = this->size;

    while (i--)
        iterator(items[i]);
}

int uclist_remove(uclist *const this, void *const data)
{
    int index = uclist_find(this, data);

    if (index >= 0)
        uclist_removeByIndex(this, index);

    return index;
}

void uclist_restore(uclist *const this, void *const data)
{
    void **const items = this->items;
    unsigned short i = this->capacity;

    while (i--)
        if (items[i] == data)
        {
            unsigned short const size = this->size;

            items[i] = items[size];
            items[size] = data;

            ++this->size;

            return;
        }
}

int uclist_find(uclist *const this, void *const data)
{
    void **const items = this->items;
    unsigned short i = this->size;

    while (i--)
        if (items[i] == data)
            return i;

    return -1;
}

unsigned short uclist_removeByIndex(uclist *const this, unsigned short index)
{
    unsigned short size = this->size;

    if (index >= size)
        return 0;

    size = --this->size;

    void **const items = this->items;
    void *const swap = items[index];

    items[index] = items[size];
    items[size] = swap;

    return 1;
}

unsigned short uclist_reset(uclist *const this)
{
    this->size = 0;

    unsigned short capacity = this->capacity;
    unsigned short const itemSize = this->itemSize;

    if (capacity == 0 || itemSize == 0)
        return 2;

    void *ptr = malloc(capacity * itemSize);

    if (!ptr)
        return 0;

    void **const items = this->items;

    while (capacity--)
    {
        free(items[capacity]);
        items[capacity] = (unsigned char *)ptr + capacity * itemSize;
    }

    return 1;
}

void uclist_end(uclist *const this)
{
    void **const items = this->items;
    unsigned short const itemSize = this->itemSize;

    if (itemSize)
    {
        unsigned short i = this->capacity;

        while (i--)
            free(items[i]);
    }

    free(items);
    uclist_init_alloc(this, itemSize);
}

//

#define FUNC(NAME, ...)                                                                       \
    static void NAME(void *list[], void (*it)(), unsigned short size, unsigned short nbItems) \
    {                                                                                         \
        for (unsigned short i = 0; i < size; i += nbItems)                                    \
            it(__VA_ARGS__);                                                                  \
    }

FUNC(f2, list[i + 0], list[i + 1]);
FUNC(f3, list[i + 0], list[i + 1], list[i + 2]);
FUNC(f4, list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
FUNC(f5, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);

int uclist_iteratorEx(uclist *const this, void (*iterator)(), unsigned short nbItems)
{
    static void (*const _exec[])() = {0, 0, f2, f3, f4, f5};

    _exec[nbItems](this->items, iterator, this->size, nbItems);

    return this->size / nbItems;
}
