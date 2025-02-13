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

//

#include "../dcalloc/dcalloc.h"
typedef dcalloc de_manager;

void de_manager_init(de_manager *const, unsigned);
de_entity *de_manager_new(de_manager *const, de_state);
int de_manager_pause_entity(de_manager *const, de_entity *const);
int de_manager_resume_entity(de_manager *const, de_entity *const);
void de_manager_update(de_manager *const);
void de_manager_reset(de_manager *const);
void de_manager_end(de_manager *const);

//

#define DARKEN_SIZE(SIZE) sizeof(de_entity) + SIZE

void darken(de_entity *const, de_state);
