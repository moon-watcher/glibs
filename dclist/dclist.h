#pragma once

#define DCLIST_NOT_FOUND -1
#define DCLIST_NULL_ITERATOR -1
#define DCLIST_NO_ITEMS -2

typedef struct dclist
{
    void **list;
    unsigned size;
    unsigned capacity;
} dclist;

void dclist_init(dclist *conss);
void *dclist_add(dclist *const, void *const);
int dclist_iterator(dclist *const, void (*)());
int dclist_remove(dclist *const, void *const);
void dclist_reset(dclist *const);
void dclist_end(dclist *const);

//

int dclist_findIndex(dclist *const, void *const);
int dclist_removeByIndex(dclist *const, unsigned);
int dclist_iteratorEx(dclist *const, void (*)(), unsigned);
