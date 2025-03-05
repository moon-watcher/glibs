#include "darken.h"

// Entity

inline de_entity *de_entity_set       (de_entity *const this, de_state state) { return this->state      = state, this; }
inline de_entity *de_entity_destructor(de_entity *const this, de_state state) { return this->destructor = state, this; }
inline de_entity *de_entity_delete    (de_entity *const this)                 { return de_entity_set(this, 0);         }
inline de_entity *de_entity_exec      (de_entity *const this)                 { return this->state(this->data, this), this; }

// Manager

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

// Darken

void darken(de_entity *const entity, de_state state)
{
    de_entity_set(entity, state);

    while (state != 0)
        state = state(entity->data, entity);
}
