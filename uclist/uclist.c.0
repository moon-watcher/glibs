// Unordered Cacheable List

#include "uclist.h"
#include <stdlib.h>

enum
{
    UCLIST_ADD,
    UCLIST_ALLOC,
    UCLIST_FIXED_ALLOC
};

void uclist_init_add(uclist *$, uint16_t capacity)
{
    *$ = (uclist){0};

    if (!capacity)
        return;

    $->items = malloc(capacity * sizeof(void *));
    $->capacity = $->items ? capacity : 0;
}

void *uclist_init_alloc(uclist *$, uint16_t itemSize, uint16_t capacity)
{
    *$ = (uclist){.itemSize = itemSize, .mode = UCLIST_ALLOC};
    if (!capacity)
        return (void *)1;

    *$ = (uclist){malloc(capacity * sizeof(void *)), 0, capacity, itemSize, UCLIST_FIXED_ALLOC};
    if (!$->items)
        return 0;

    void *block = malloc(capacity * itemSize);
    if (!block)
    {
        free($->items);
        $->items = 0;
        return 0;
    }

    for (uint16_t i = 0; i < capacity; i++)
        $->items[i] = (uint8_t *)block + i * itemSize;

    return $->items[0];
}

void *uclist_alloc(uclist *$)
{
    if ($->mode == UCLIST_ADD)
        return 0;

    if ($->size < $->capacity)
    {
        void *ptr = $->items[$->size++];
        memset(ptr, 0, $->itemSize);
        return ptr;
    }

    if ($->mode == UCLIST_FIXED_ALLOC)
        return 0;

    void *ptr = malloc($->itemSize);
    if (!ptr)
        return 0;

    memset(ptr, 0, $->itemSize);
    if (!uclist_add($, ptr))
    {
        free(ptr);
        return 0;
    }

    return ptr;
}

uint16_t uclist_add(uclist *$, void *add)
{
    if ($->mode == UCLIST_FIXED_ALLOC)
        return 0;

    if ($->size >= $->capacity)
    {
        void **ptr = malloc(($->capacity + 1) * sizeof(void *));
        if (!ptr)
            return 0;

        memcpy(ptr, $->items, $->capacity * sizeof(void *));
        free($->items);

        $->items = ptr;
        ++$->capacity;
    }

    $->items[$->size++] = add;

    return 1;
}

uint16_t uclist_remove(uclist *$, void *data)
{
    uint16_t i = $->size;

    while (i--)
        if ($->items[i] == data)
        {
            --$->size;

            void **items = $->items;
            void *swap = items[i];

            items[i] = items[$->size];
            items[$->size] = swap;

            return 1;
        }

    return 0;
}

uint16_t uclist_restore(uclist *$, void *data)
{
    uint16_t i = $->capacity;

    while (i > $->size)
        if ($->items[--i] == data)
        {
            $->items[i] = $->items[$->size];
            $->items[$->size++] = data;

            return 1;
        }

    return 0;
}

void uclist_reset(uclist *$)
{
    $->size = 0;
}

void uclist_end(uclist *$)
{
    if ($->items)
    {
        if ($->mode == UCLIST_FIXED_ALLOC)
            free($->items[0]);

        else if ($->mode == UCLIST_ALLOC)
            while ($->capacity--)
                free($->items[$->capacity]);

        free($->items);
    }

    *$ = (uclist){0};
}

//

void uclist_iterator(uclist *$, void (*iterator)())
{
    uint16_t i = $->size;

    while (i--)
        iterator($->items[i]);
}

#define FUNC(NAME, ...)                                                           \
    static void NAME(void *list[], void (*it)(), uint16_t size, uint16_t nbItems) \
    {                                                                             \
        for (uint16_t i = 0; i < size; i += nbItems)                              \
            it(__VA_ARGS__);                                                      \
    }

FUNC(f2, list[i + 0], list[i + 1]);
FUNC(f3, list[i + 0], list[i + 1], list[i + 2]);
FUNC(f4, list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
FUNC(f5, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);

static void (*_iteratorEx[])() = {0, 0, f2, f3, f4, f5};

uint16_t uclist_iteratorEx(uclist *$, void (*iterator)(), uint16_t nbItems)
{
    if (nbItems < 2 || nbItems > 5)
        return 0;
    // if ($->size % nbItems != 0)
    //     return 0;

    _iteratorEx[nbItems]($->items, iterator, $->size, nbItems);

    return $->size / nbItems;
}
