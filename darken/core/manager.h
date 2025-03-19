#pragma once

#include "state.h"
#include "entity.h"

#include "../alloc/alloc.h"

typedef de_alloc de_manager;

void       de_manager_init   (de_manager *const, unsigned);
de_entity *de_manager_new    (de_manager *const, de_state, de_state);
unsigned   de_manager_update (de_manager *const);
int        de_manager_reset  (de_manager *const);
int        de_manager_end    (de_manager *const);
