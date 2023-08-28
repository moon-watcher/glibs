#pragma once

#include "node.h"

typedef void (*ListFree_f)(void *const);
typedef void (*ListIterator_f)();

typedef struct
{
	unsigned length;
	unsigned elementSize;
	ListNode_t *head;
	ListNode_t *tail;
	ListNode_t *current;
	ListFree_f freeFn;
} List_t;

void        list_init    ( List_t *const, unsigned, ListFree_f );
void        list_destroy ( List_t *const );
unsigned    list_size    ( List_t *const );
ListNode_t *list_prepend ( List_t *const, void *const );
ListNode_t *list_append  ( List_t *const, void *const );
ListNode_t *list_head    ( List_t *const, void *const, unsigned );
ListNode_t *list_tail    ( List_t *const, void *const );
void        list_foreach ( List_t *const, ListIterator_f );
ListNode_t *list_index   ( List_t *const, unsigned );
unsigned    list_remove  ( List_t *const, ListNode_t* );

#define list_getData(NODE) \
	NODE->data

#define list_current(LIST) \
	({LIST->current = LIST->current->next; LIST->current->prev;})

#define list_rewind(LIST) \
	(LIST->current = LIST->head)

#define list_next(LIST, NODE) \
	(LIST->current = NODE->next)

#define list_prev(LIST, NODE) \
	(LIST->current = NODE->prev)
