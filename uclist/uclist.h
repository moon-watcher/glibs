#pragma once

typedef struct uclist
{
    void **items;
    unsigned size;
    unsigned capacity;
    unsigned itemSize;
} uclist;

void uclist_init_alloc(uclist *const, unsigned);
void uclist_init_add(uclist *const);
void *uclist_alloc(uclist *const);
void *uclist_add(uclist *const, void *const);
void uclist_iterator(uclist *const, void (*)());
int uclist_remove(uclist *const, void *const);
void uclist_restore(uclist *const, void *const);
int uclist_find(uclist *const, void *const);
unsigned uclist_removeByIndex(uclist *const, unsigned);
unsigned uclist_reset(uclist *const);
void uclist_end(uclist *const);

//

int uclist_iteratorEx(uclist *const, void (*)(), unsigned);
