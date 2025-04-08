#pragma once

#include "../list/list.h"

typedef struct
{
    de_list list;
    void (*update_f)();
    unsigned params;
} de_system;

void de_system_init(de_system *const, void (*)(), unsigned);
void *de_system_add(de_system *const, void *const);
int de_system_delete(de_system *const, void *const);
void de_system_update(de_system *const);
void de_system_reset(de_system *const);
void de_system_end(de_system *const);
