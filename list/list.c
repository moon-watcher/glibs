// Borrowed from http://pseudomuto.com/development/2013/05/02/implementing-a-generic-linked-list-in-c/ Thanks!

#include "list.h"
#include "config.h"

static void remove(List_t *list, ListNode_t *node)
{
	if (list->freeFn)
		list->freeFn(node->data);

	list->length--;

	free(node->data);
	free(node);
}

void list_init(List_t *const list, unsigned elementSize, ListFree_f freeFn)
{
	list->length = 0;
	list->elementSize = elementSize;
	list->head = NULL;
	list->tail = NULL;
	list->current = NULL;
	list->freeFn = freeFn;
}

void list_destroy(List_t *const list)
{
	ListNode_t *node;

	while (list->head != NULL)
	{
		node = list->head;
		list->head = node->next;

		remove(list, node);
	}
}

/** 
 * @todo node->prev to be implemented
 * @example https://www.programiz.com/dsa/doubly-linked-list
*/
ListNode_t *list_prepend(List_t *const list, void *const element)
{
	ListNode_t *node = malloc(sizeof(ListNode_t));
	node->data = malloc(list->elementSize);
	memcpy(node->data, element, list->elementSize);

	node->next = list->head;
	node->list = list;
	list->head = node;

	if (!list->tail)
		list->tail = list->head;

	list->length++;
	list->current = node;

	return node;
}

ListNode_t *list_append(List_t *const list, void *const element)
{
	ListNode_t *node = malloc(sizeof(ListNode_t));
	node->data = malloc(list->elementSize);
	node->next = NULL;
	node->list = list;

	memcpy(node->data, element, list->elementSize);

	if (list->length)
	{
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
	}
	else
	{
		list->tail = node;
		list->head = node;
		node->prev = NULL;
	}

	list->length++;
	list->current = node;

	return node;
}

void list_foreach(List_t *const list, ListIterator_f iterator)
{
	ListNode_t *node = list->head;

	while (node)
	{
		iterator(node->data);
		node = node->next;
	}
}

ListNode_t *list_index(List_t *const list, unsigned index)
{
	ListNode_t *node = list->head;
	unsigned i = 0;

	while (node)
	{
		if (i++ == index)
			return node;

		node = node->next;
	}

	return NULL;
}

ListNode_t *list_head(List_t *const list, void *const element, unsigned removeFromList)
{
	ListNode_t *node = list->head;
	memcpy(element, node->data, list->elementSize);

	list->current = node;
	
	if (!removeFromList)
		return NULL;
	
	list->head = node->next;
	list->current = list->head;

	remove(list, node);

	return list->head;
}

/** 
 * @todo node->prev to be implemented
 * @example https://www.programiz.com/dsa/doubly-linked-list
*/
ListNode_t *list_tail(List_t *const list, void *element)
{
	ListNode_t *node = list->tail;
	memcpy(element, node->data, list->elementSize);
	list->current = list->tail;

	return list->tail;
}

unsigned list_size(List_t *const list)
{
	return list->length;
}

/** 
 * @todo node->prev to be implemented
 * @attention Use to avoid search
 * @example https://www.programiz.com/dsa/doubly-linked-list
*/
unsigned list_remove(List_t *const list, ListNode_t *const search)
{
	ListNode_t *prev = NULL;
	ListNode_t *node = list->head;

	while (node)
	{
		if (search == node)
		{
			if (node == list->head)
				list->head = node->next;
			else if (node == list->tail)
				list->tail = prev;
			else
				prev->next = node->next;

			remove(list, node);

			return 1;
		}

		prev = node;
		list->current = node;
		node = node->next;
	}

	return 0;
}