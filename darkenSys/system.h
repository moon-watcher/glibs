#pragma once

#include "../dclist/dclist.h"

typedef dclist darkenSys;

void darkenSys_init(darkenSys *const);
void *darkenSys_add(darkenSys *const, void *const);
int darkenSys_delete(darkenSys *const, void *const);
int darkenSys_update(darkenSys *const, void (*)(), unsigned);
void darkenSys_reset(darkenSys *const);
void darkenSys_end(darkenSys *const);
