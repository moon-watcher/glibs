#pragma once

#define DCALLOC_NOT_FOUND -1
#define DCALLOC_NULL_ITERATOR -1
#define DCALLOC_NO_ITEMS -2

typedef struct
{
    void **list;
    unsigned size;
    unsigned capacity;
    unsigned itemSize;
    // unsigned ordered;
} dcalloc;

void dcalloc_init(dcalloc *const, unsigned);
void *dcalloc_alloc(dcalloc *const);
int dcalloc_iterator(dcalloc *const, void (*)());
int dcalloc_remove(dcalloc *const, void *const);
int dcalloc_restore(dcalloc *const, void *const);
void dcalloc_reset(dcalloc *const);
void dcalloc_end(dcalloc *const);

//

int dcalloc_findIndex(dcalloc *const, void *const);
int dcalloc_removeByIndex(dcalloc *const, unsigned);
int dcalloc_iteratorEx(dcalloc *const, void (*)(), unsigned);
