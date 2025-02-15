#include "darken.h"
#include "config.h"

// Entity

de_entity *de_entity_set(de_entity *const this, de_state state)
{
    if (this != 0)
        return this->state = state, this;

    DARKEN_LOG("invalid entity");
    return this;
}

de_entity *de_entity_destructor(de_entity *const this, de_state state)
{
    if (this != 0)
        return this->destructor = state, this;

    DARKEN_LOG("invalid entity");
    return this;
}

de_entity *de_entity_delete(de_entity *const this)
{
    return de_entity_set(this, 0);
}

// Manager

void de_manager_init(de_manager *const this, unsigned bytes)
{
    dcalloc_init(this, DARKEN_SIZE(bytes));
}

de_entity *de_manager_new(de_manager *const this, de_state state)
{
    return de_entity_set(dcalloc_alloc(this), state);
}

int de_manager_pause_entity(de_manager *const this, de_entity *const entity)
{
    return dcalloc_remove(this, entity);
}

int de_manager_resume_entity(de_manager *const this, de_entity *const entity)
{
    return dcalloc_restore(this, entity);
}

void de_manager_update(de_manager *const this)
{
    for (unsigned i = 0; i < this->size;)
    {
        de_entity *const entity = this->list[i++];

        if (entity->state == 0)
        {
            dcalloc_removeByIndex(this, --i);
            de_entity_set(entity, entity->destructor ?: ({ void *f(){return 0;} f; }));
        }

        entity->state = entity->state(entity->data, entity);
    }
}

void de_manager_reset(de_manager *const this)
{
    dcalloc_iterator(this, (void *)de_entity_delete);
    de_manager_update(this);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    dcalloc_end(this);
}

// Darken

void darken(de_entity *const entity, de_state state)
{
    de_entity_set(entity, state);

    while (state != 0)
        state = state(entity->data, entity);
}
