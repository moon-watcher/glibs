#pragma once

#include "state.h"

#include "../alloc/alloc.h"
typedef de_alloc de_manager;

typedef struct de_entity
{
    de_state state;
    de_state destructor;
    de_manager *manager;

    unsigned char data[]; // Bytes for data
} de_entity;

de_entity *de_entity_set        (de_entity *const, de_state);
de_entity *de_entity_destructor (de_entity *const, de_state);
de_entity *de_entity_delete     (de_entity *const);
de_entity *de_entity_pause      (de_entity *const);
de_entity *de_entity_resume     (de_entity *const);
void      *de_entity_exec       (de_entity *const);
