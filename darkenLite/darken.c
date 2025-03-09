#include "darken.h"


// State

void *de_state_empty()
{
    return de_state_empty;
}


// Entity

inline de_entity *de_entity_set(de_entity *const this, de_state state)
{
    if (this)
        this->state = state;

    return this;
}

inline de_entity *de_entity_destructor(de_entity *const this, de_state state)
{
    if (this)
        this->destructor = state;

    return this;
}

inline de_entity *de_entity_delete(de_entity *const this)
{
    if (this)
        this->state = 0;

    return this;
}

inline de_entity *de_entity_pause(de_entity *const this)
{
    if (this && this->manager && dcalloc_remove(this->manager, this) > DCALLOC_NOT_FOUND)
        return this;
    
    return 0;
}

inline de_entity *de_entity_resume (de_entity *const this)
{
    if (this && this->manager && dcalloc_restore(this->manager, this) > DCALLOC_NOT_FOUND)
        return this;

    return 0;
}

inline void *de_entity_exec(de_entity *const this)
{
    if (this && this->state)
        return this->state(this->data, this);

    return 0;
}


// Manager

void de_manager_init(de_manager *const this, unsigned bytes)
{
    dcalloc_init(this, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, de_state state)
{
    de_entity *const entity = dcalloc_alloc(this);
    entity->manager = this;

    return de_entity_set(entity, state);
}

unsigned de_manager_update(de_manager *const this)
{
    for (unsigned i = 0; i < this->size;)
    {
        de_entity *const entity = this->list[i++];

        if (entity->state == 0)
        {
            dcalloc_removeByIndex(this, --i);
            de_entity_set(entity, entity->destructor ?: de_state_empty);
        }

        entity->state = de_entity_exec(entity);
    }

    return this->size;
}

int de_manager_reset(de_manager *const this)
{
    int ret = dcalloc_iterator(this, de_entity_delete);

    if (ret > DCALLOC_NULL_ITERATOR)
        de_manager_update(this);

    return ret;
}

int de_manager_end(de_manager *const this)
{
    int ret = de_manager_reset(this);

    if (ret > DCALLOC_NULL_ITERATOR)
        dcalloc_end(this);

    return ret;
}
