#include "manager.h"

void de_manager_init(de_manager *const this, unsigned bytes)
{
    dcalloc_init(this, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, de_state state, de_state destructor)
{
    de_entity *const entity = dcalloc_alloc(this);
    entity->manager = this;

    return de_entity_destructor(de_entity_set(entity, state), destructor);
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
    int ret = dcalloc_iterator(this, (void *)de_entity_delete); 

    if (ret > 0)
        de_manager_update(this);

    return ret;
}

int de_manager_end(de_manager *const this)
{
    int ret = de_manager_reset(this);

    if (ret > 0)
        dcalloc_end(this);

    return ret;
}
