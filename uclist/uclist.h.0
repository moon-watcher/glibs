#pragma once

#include <stdint.h>

typedef struct
{
    void **items;
    uint16_t size;
    uint16_t capacity;
    uint16_t itemSize;
    uint8_t mode;
} uclist;

void uclist_init_add(uclist *, uint16_t);
void *uclist_init_alloc(uclist *, uint16_t, uint16_t);
void *uclist_alloc(uclist *);
uint16_t uclist_add(uclist *, void *);
uint16_t uclist_remove(uclist *, void *);
uint16_t uclist_restore(uclist *, void *);
void uclist_reset(uclist *);
void uclist_end(uclist *);

void uclist_iterator(uclist *, void (*)());
uint16_t uclist_iteratorEx(uclist *, void (*)(), uint16_t);

#define UCLIST_MEM_PTRS(U) ((U)->capacity * sizeof(void *))
#define UCLIST_MEM_USED(U) ((U)->itemSize * (U)->size)
#define UCLIST_MEM_FREE(U) ((U)->itemSize * ((U)->size <= (U)->capacity ? (U)->capacity - (U)->size : 0))
#define UCLIST_MEM_TOTAL(U) ((U)->itemSize * (U)->capacity)
