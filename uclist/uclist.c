// Unordered Cacheable List

#include "uclist.h"
#include "config.h"
#include "assert.h"

void uclist_init(uclist *const this, unsigned maxItemSize)
{
    memset(this, 0, sizeof(uclist));
    this->itemSize = maxItemSize;
}

void *uclist_alloc(uclist *const this)
{
    void *ptr = this->list[this->size];

    if (this->size < this->capacity)
    {
        ++this->size;
    }
    else if ((ptr = malloc(this->itemSize)) != 0)
    {
        ptr = uclist_add(this, ptr);
    }

    memset(ptr, 0, this->itemSize);
    return ptr;
}

void *uclist_add(uclist *const this, void *const add)
{
    if (add != 0)
    {
        if (this->size >= this->capacity)
        {
            void *ptr = malloc((this->capacity + 1) * sizeof(void *));

            UCLIST_ASSERT(ptr == 0, 0);

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

int uclist_iterator(uclist *const this, void (*iterator)())
{
    UCLIST_ASSERT(iterator == 0, UCLIST_NULL_ITERATOR);

    for (unsigned i = 0; i < this->size; ++i)
    {
        iterator(this->list[i]);
    }

    return this->size;
}

int uclist_remove(uclist *const this, void *const data, void *(*exec)())
{
    for (unsigned i = 0; i < this->capacity; i++)
    {
        if (this->list[i] == data)
        {
            exec != 0 && exec(data);

            --this->size;
            this->list[i] = this->list[this->size];
            this->list[this->size] = data;
            return i;
        }
    }

    return UCLIST_NOT_FOUND;
}

void uclist_reset(uclist *const this)
{
    this->size = 0;
}

void uclist_end(uclist *const this)
{
    while (this->itemSize && this->capacity--)
    {
        free(this->list[this->capacity]);
    }

    free(this->list);
    uclist_init(this, this->itemSize);
}
