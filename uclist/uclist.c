// Unordered Cacheable List

#include "uclist.h"

void uclist_init(uclist *$, uint16_t itemSize)
{
    $->items = 0;
    $->size = 0;
    $->capacity = 0;
    $->itemSize = itemSize;
}

void *uclist_alloc(uclist *$)
{
    void *ptr = $->items[$->size];

    if ($->size < $->capacity)
        $->size++;

    else if (!((ptr = malloc($->itemSize)) && uclist_add($, ptr)))
        free(ptr);

    memset(ptr, 0, $->itemSize);

    return ptr;
}

void *uclist_add(uclist *$, void *add)
{
    if ($->size >= $->capacity)
    {
        void **ptr = malloc(($->capacity + 1) * sizeof(void *));
        if (!ptr) return 0;

        memcpy(ptr, $->items, $->capacity * sizeof(void *));
        free($->items);

        $->items = ptr;
        ++$->capacity;
    }

    return $->items[$->size++] = add;
}

void uclist_iterator(uclist *$, void (*iterator)())
{
    uint16_t i = $->size;

    while (i--)
        iterator($->items[i]);
}

uint16_t uclist_remove(uclist *$, void *data)
{
    return uclist_removeByIndex($, (uint16_t)uclist_getIndex($, data));
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

uint16_t uclist_reset(uclist *$)
{
    $->size = 0;

    if ($->capacity == 0) return 3;
    if ($->itemSize == 0) return 2;

    void *ptr = malloc($->capacity * $->itemSize);
    if (!ptr) return 0;

    uint16_t i = $->capacity;

    while (i--)
    {
        free($->items[i]);
        $->items[i] = (uint8_t *)ptr + i * $->itemSize;
    }

    return 1;
}

void uclist_end(uclist *$)
{
    while ($->itemSize && $->capacity--)
        free($->items[$->capacity]);

    free($->items);
    uclist_init($, $->itemSize);
}

//

void *uclist_addSafe(uclist *$, void *add)
{
    return (uclist_getIndex($, add) < 0) ? uclist_add($, add) : 0;
}

int16_t uclist_getIndex(uclist *$, void *data)
{
    uint16_t i = $->size;

    while (i--)
        if ($->items[i] == data)
            return i;

    return -1;
}

uint16_t uclist_removeByIndex(uclist *$, uint16_t index)
{
    if (index >= $->size) return 0;

    --$->size;

    void **items = $->items;
    void *swap = items[index];

    items[index] = items[$->size];
    items[$->size] = swap;

    return 1;
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
    _iteratorEx[nbItems]($->items, iterator, $->size, nbItems);

    return $->size / nbItems;
}
