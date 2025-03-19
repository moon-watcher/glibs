#pragma once

#define DE_ALLOC_NOT_FOUND -1
#define DE_ALLOC_NULL_ITERATOR -1
#define DE_ALLOC_NO_ITEMS -2

typedef struct
{
    void **list;
    unsigned size;
    unsigned capacity;
    unsigned itemSize;
    // unsigned ordered;
} de_alloc;

void de_alloc_init(de_alloc *const, unsigned);
void *de_alloc_alloc(de_alloc *const);
int de_alloc_iterator(de_alloc *const, void (*)());
int de_alloc_remove(de_alloc *const, void *const);
int de_alloc_restore(de_alloc *const, void *const);
void de_alloc_reset(de_alloc *const);
void de_alloc_end(de_alloc *const);

//

int de_alloc_findIndex(de_alloc *const, void *const);
int de_alloc_removeByIndex(de_alloc *const, unsigned);
int de_alloc_iteratorEx(de_alloc *const, void (*)(), unsigned);
