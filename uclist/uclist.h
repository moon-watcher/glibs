#pragma once

#define UCLIST_NOT_FOUND -1
#define UCLIST_NULL_ITERATOR -1
#define UCLIST_NO_ITEMS -2

typedef struct uclist
{
    void **list;
    unsigned size;
    unsigned capacity;
    unsigned itemSize;
} uclist;

void uclist_init(uclist *const, unsigned);
void *uclist_alloc(uclist *const);
void *uclist_add(uclist *const, void *const);
int uclist_iterator(uclist *const, void (*)());
int uclist_remove(uclist *const, void *const, void *(*exec)());
void uclist_reset(uclist *const);
void uclist_end(uclist *const);

//

int uclist_findIndex(uclist *const, void *const);
void uclist_removeByIndex(uclist *const, unsigned, void (*)());
int uclist_iteratorEx(uclist *const, void (*)(), unsigned);
