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

void de_entity_set        (de_entity *const, de_state);
void de_entity_destructor (de_entity *const, de_state);
void de_entity_delete     (de_entity *const);
void de_entity_exec       (de_entity *const);


// Manager

#include "../dcalloc/dcalloc.h"
typedef dcalloc de_manager;

void de_manager_update(de_manager *const);

#define de_manager_init(THIS, BYTES) dcalloc_init(THIS, sizeof(de_entity) + BYTES)
#define de_manager_new(THIS, STATE)  ({void *const e = dcalloc_alloc(THIS); de_entity_set(e, STATE); e;})
#define de_manager_pause_entity      dcalloc_remove
#define de_manager_resume_entity     dcalloc_restore
#define de_manager_reset(THIS)       dcalloc_iterator(THIS, de_entity_delete); de_manager_update(THIS)
#define de_manager_end(THIS)         de_manager_reset(THIS); dcalloc_end(THIS)
