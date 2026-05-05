#include "ffpAlloc.h"
#include <stdlib.h>

// ffpAlloc: First Fit Pool Allocator

uint16_t ffpAlloc_new(ffpAlloc_t *a, uint16_t chunk_size)
{
	uint16_t pos = a->base;
	struct ffpAllocNode *node = a->head;
	struct ffpAllocNode *new = malloc(sizeof(struct ffpAllocNode));
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

void ffpAlloc_delete(ffpAlloc_t *a, uint16_t pos)
{
	if (pos < a->base)
		return;

	struct ffpAllocNode *node = a->head;
	struct ffpAllocNode *prev = 0;

	while (node)
	{
		if (node->index == pos)
		{
			if (prev)
				prev->next = node->next;
			else
				a->head = node->next;

			free(node);
			--a->count;

			return;
		}

		prev = node;
		node = node->next;
	}
}

void ffpAlloc_destroy(ffpAlloc_t *a)
{
	while (a->head)
	{
		struct ffpAllocNode *aux = a->head->next;

		free(a->head);
		a->head = aux;
	}

	*a = (ffpAlloc_t){0};
}

// uint16_t ffpAlloc_count(ffpAlloc_t *a)
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
//	ffpAlloc_new(9);
//	s16 x = ffpAlloc_new(3);
//	ffpAlloc_new(927);
//	ffpAlloc_delete ( x );
//	ffpAlloc_new(2);
//	uint16_t y = ffpAlloc_new(5);
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
