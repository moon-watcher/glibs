#include "entity.h"

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
    if (this && this->manager && de_alloc_remove(this->manager, this) >= 0)
        return this;

    return 0;
}

inline de_entity *de_entity_resume(de_entity *const this)
{
    if (this && this->manager && de_alloc_restore(this->manager, this) >= 0)
        return this;

    return 0;
}

inline void *de_entity_exec(de_entity *const this)
{
    if (this && this->state)
        return this->state(this->data, this);

    return 0;
}
