#pragma once

typedef struct uclist
{
    void **items;
    unsigned short size;
    unsigned short capacity;
    unsigned short itemSize;
} uclist;

void uclist_init_alloc(uclist *const, unsigned short);
void uclist_init_add(uclist *const);
void *uclist_alloc(uclist *const);
void *uclist_add(uclist *const, void *const);
void uclist_iterator(uclist *const, void (*)());
int uclist_remove(uclist *const, void *const);
void uclist_restore(uclist *const, void *const);
int uclist_find(uclist *const, void *const);
unsigned short uclist_removeByIndex(uclist *const, unsigned short);
unsigned short uclist_reset(uclist *const);
void uclist_end(uclist *const);

//

int uclist_iteratorEx(uclist *const, void (*)(), unsigned short);
