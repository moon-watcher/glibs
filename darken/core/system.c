#include "system.h"

void de_system_init(de_system *const this, void (*update)(), unsigned params)
{
    this->update_f = update;
    this->params = params;
    de_list_init(&this->list);
}

void *de_system_add(de_system *const this, void *const data)
{
    return de_list_add(&this->list, data);
}

int de_system_delete(de_system *const this, void *const data)
{
    return de_list_remove(&this->list, data);
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
