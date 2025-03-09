#pragma once


// State

typedef void *(*de_state)();

void *de_state_empty();


// Manager

#include "../dcalloc/dcalloc.h"
typedef dcalloc de_manager;


// Entity

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


// Manager

void       de_manager_init   (de_manager *const, unsigned);
de_entity *de_manager_new    (de_manager *const, de_state);
unsigned   de_manager_update (de_manager *const);
int        de_manager_reset  (de_manager *const);
int        de_manager_end    (de_manager *const);
