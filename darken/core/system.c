#include "system.h"
#include "../config.h"

void de_system_init(de_system *const this, void (*update)(), unsigned params)
{
    this->update_f = update;
    this->params = params;
    de_list_init(&this->list);
}

unsigned de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    de_list *const list = &this->list;

    for (unsigned i = 0; i < params; i++)
        if (0 == de_list_add(list, va_arg(ap, void *const)))
            return 0;

    return 1;
}

unsigned de_system_delete(de_system *const this, void *const data)
{
    int index = de_list_findIndex(&this->list, data);

    if (index < 0)
        return 0;

    de_list *const list = &this->list;
    list->size -= this->params;

    memcpy(list->items[index], list->items[list->size], sizeof(void *) * this->params);

    return 1;
}

void de_system_update(de_system *const this)
{
    de_list_iteratorEx(&this->list, this->update_f, this->params);
}

void de_system_reset(de_system *const this)
{
    de_list_reset(&this->list);
}

void de_system_end(de_system *const this)
{
    de_list_end(&this->list);
}

/*

void *de_system_add(de_system *const this, void *const data)
{
    return de_list_add(&this->list, data);
}

int de_system_delete(de_system *const this, void *const data)
{
    return de_list_remove(&this->list, data);
}



void de_system_addEx(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);

    for (int i = 0; i < this->params; i++)
        de_list_add(&this->list, va_arg(ap, void *const));
}

void de_system_deleteEx(de_system *const this, void *const data)
{
    int index = uplist_find(&this->list, data);
    if (index < 0) return;

    this->list.count -= this->params;
    memcpy(this->list.items[index], this->list.items[this->list.count], sizeof(void *) * this->params);
}



int de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uclist *const list = &this->list;

    for (unsigned i = 0; i < params; i++)
    {
        if (0 == uclist_add(list, va_arg(ap, void *const)))
        {
            DARKEN_LOG("system add reference");
            return 0;
        }
    }

    return 1;
}

int de_system_delete(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uclist *const list = &this->list;
    int ret = 1;

    for (unsigned i = 0; i < params; i++)
    {
        switch (uclist_remove(list, va_arg(ap, void *const), 0))
        {
#if DARKEN_LOG
        case -1:
            ret = -1;
            DARKEN_LOG("system, ref not found");
            break;
        case -2:
            ret = -2;
            DARKEN_LOG("system, this->count");
            break;
#endif
        }
    }

    return ret;
}
*/