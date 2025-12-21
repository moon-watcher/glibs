#include "ffpAlloc.h"

// ffpAlloc: First Fit Pool Allocator

void ffpAlloc_init(ffpAlloc_t *const a, void *(*malloc)(unsigned), void (*free)(void *))
{
	a->head = 0;
	a->base = 0;
	a->count = 0;
	a->malloc = malloc;
	a->free = free;
}

void ffpAlloc_destroy(ffpAlloc_t *const a)
{
	while (a->head)
	{
		struct ffpAllocNode *aux = a->head->next;

		a->free(a->head);
		a->head = aux;
	}

	a->head = 0;
	a->base = 0;
	a->count = 0;
}

unsigned ffpAlloc_new(ffpAlloc_t *const a, unsigned chunk_size)
{
	unsigned pos = a->base;
	struct ffpAllocNode *node = a->head;
	struct ffpAllocNode *new = a->malloc(sizeof(struct ffpAllocNode));
	struct ffpAllocNode *next = 0;

	if (node)
	{
		while (node)
		{
			pos = node->index + node->size;
			next = node->next;

			if (next == 0 || next->index >= pos + chunk_size)
				break;

			node = next;
		}

		node->next = new;
	}
	else
		a->head = new;

	new->index = pos;
	new->size = chunk_size;
	new->next = next;

	++a->count;

	return pos;
}

void ffpAlloc_delete(ffpAlloc_t *const a, unsigned pos)
{
	if (pos < a->base)
		return;

	struct ffpAllocNode *node = a->head;
	struct ffpAllocNode *prev = 0;

	while (node)
	{
		if (node->index == pos)
		{
			prev->next = node->next;
			a->free(node);

			if (--a->count == 0)
				a->head = 0;

			return;
		}

		prev = node;
		node = node->next;
	}
}

// unsigned ffpAlloc_count(ffpAlloc_t *const a)
// {
// 	return a->count;
// }

// void ffpAlloc_info ()
//{
//	struct ffpAllocNode *aux = head;
//
//	char i = 3;
//	char str[10];
//
//	VDP_drawText ( " Nb   Pos   Tiles ", 1, 0 );
//	VDP_drawText ( "---- ----- -------", 1, 1 );
//
//	while ( aux )
//	{
//		uintToStr(i-2, str, 1);
//		VDP_drawText(str, 2, i);
//
//		uintToStr(aux->pos, str, 1);
//		VDP_drawText(str, 7, i);
//
//		uintToStr(aux->size, str, 1);
//		VDP_drawText(str, 13, i);
//
//		aux = aux->next;
//		i++;
//	}
// }

// void ffpAlloc_sample ( void )
//{
//	ffpAlloc_init(0);
//
//	ffpAlloc_new(9);
//	s16 x = ffpAlloc_new(3);
//	ffpAlloc_new(927);
//	ffpAlloc_delete ( x );
//	ffpAlloc_new(2);
//	unsigned y = ffpAlloc_new(5);
//	ffpAlloc_new(1);
//	ffpAlloc_new(8);
//	ffpAlloc_delete ( y );
//	ffpAlloc_new(3);
//	ffpAlloc_new(2);
//	ffpAlloc_new(1);
//	ffpAlloc_new(3);
//
//	ffpAlloc_info();
// }
