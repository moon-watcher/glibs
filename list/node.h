#pragma once

typedef struct ListNode_t
{
	void *data;
	struct ListNode_t *next;
	struct ListNode_t *prev;
	struct List_t *list;
} ListNode_t;