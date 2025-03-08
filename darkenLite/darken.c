#include "darken.h"

// State

void *de_state_empty() { return de_state_empty; }


// Entity

inline void de_entity_set       (de_entity *const this, de_state state) { this->state      = state; }
inline void de_entity_destructor(de_entity *const this, de_state state) { this->destructor = state; }
inline void de_entity_delete    (de_entity *const this)                 { this->state      = 0; }
inline void de_entity_exec      (de_entity *const this)                 { this->state(this->data, this); }


// Manager

void de_manager_update(de_manager *const this)
{
    for (unsigned i = 0; i < this->size;)
    {
        de_entity *const entity = this->list[i++];

        if (entity->state == 0)
        {
            dcalloc_removeByIndex(this, --i);
            de_entity_set(entity, entity->destructor ?: de_state_empty);
        }

        entity->state = entity->state(entity->data, entity);
    }
}
