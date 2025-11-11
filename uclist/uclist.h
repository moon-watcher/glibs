#pragma once

#include "config.h"

typedef struct
{
    void **items;
    uint16_t size;
    uint16_t capacity;
    uint16_t itemSize;
} uclist;

void uclist_init(uclist *, uint16_t);
void *uclist_alloc(uclist *);
void *uclist_add(uclist *, void *);
void uclist_iterator(uclist *, void (*)());
uint16_t uclist_remove(uclist *, void *);
uint16_t uclist_restore(uclist *, void *);
uint16_t uclist_reset(uclist *);
void uclist_end(uclist *);

void *uclist_addSafe(uclist *, void *);
int16_t uclist_getIndex(uclist *, void *);
uint16_t uclist_removeByIndex(uclist *, uint16_t);
uint16_t uclist_iteratorEx(uclist *, void (*)(), uint16_t);
