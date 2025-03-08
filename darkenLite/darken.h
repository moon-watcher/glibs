#pragma once

// State

typedef void *(*de_state)();

void *de_state_empty();


// Entity

typedef struct de_entity
{
    de_state state;
    de_state destructor;

    unsigned char data[]; // Bytes for data
} de_entity;

de_entity *de_entity_set        (de_entity *const, de_state);
de_entity *de_entity_destructor (de_entity *const, de_state);
de_entity *de_entity_delete     (de_entity *const);
de_entity *de_entity_exec       (de_entity *const);


// Manager

#include "../dcalloc/dcalloc.h"
typedef dcalloc de_manager;

void de_manager_update(de_manager *const);

#define de_manager_init(this, bytes) dcalloc_init(this, sizeof(de_entity) + bytes)
#define de_manager_new(this, state)  de_entity_set(dcalloc_alloc(this), state)
#define de_manager_pause_entity      dcalloc_remove
#define de_manager_resume_entity     dcalloc_restore
#define de_manager_reset(this)       dcalloc_iterator(this, (void *)de_entity_delete); de_manager_update(this)
#define de_manager_end(this)         de_manager_reset(this); dcalloc_end(this)
