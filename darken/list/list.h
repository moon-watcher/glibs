#pragma once

#define DE_LIST_NOT_FOUND -1
#define DE_LIST_NULL_ITERATOR -1
#define DE_LIST_NO_ITEMS -2

typedef struct de_list
{
    void **list;
    unsigned size;
    unsigned capacity;
    // unsigned ordered;
} de_list;

void de_list_init(de_list *conss);
void *de_list_add(de_list *const, void *const);
int de_list_iterator(de_list *const, void (*)());
int de_list_remove(de_list *const, void *const);
void de_list_reset(de_list *const);
void de_list_end(de_list *const);

//

int de_list_findIndex(de_list *const, void *const);
int de_list_removeByIndex(de_list *const, unsigned);
void de_list_iteratorEx(de_list *const, void (*)(), unsigned);
