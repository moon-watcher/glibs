#pragma once

typedef void *(*de_state)();

typedef struct de_entity
{
    de_state state;
    de_state destructor;

    unsigned char data[]; // Bytes for data
} de_entity;

de_entity *de_entity_set(de_entity *const, de_state);
de_entity *de_entity_destructor(de_entity *const, de_state);
de_entity *de_entity_delete(de_entity *const);
de_entity *de_entity_exec(de_entity *const);

//

#include "../dcalloc/dcalloc.h"
typedef dcalloc de_manager;

void de_manager_update(de_manager *const);

//

#define DARKEN_SIZE(SIZE) sizeof(de_entity) + SIZE

void darken(de_entity *const, de_state);

//

#define de_manager_init(this, bytes) dcalloc_init(this, DARKEN_SIZE(bytes))
#define de_manager_new(this, state) de_entity_set(dcalloc_alloc(this), state)
#define de_manager_pause_entity(this, entity) dcalloc_remove(this, entity)
#define de_manager_resume_entity(this, entity) dcalloc_restore(this, entity)
#define de_manager_reset(this) dcalloc_iterator(this, (void *)de_entity_delete); de_manager_update(this)
#define de_manager_end(this) de_manager_reset(this); dcalloc_end(this)
