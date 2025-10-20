#include "allocator.h"

void allocator_init(allocator *const a, unsigned base, void *(*malloc)(unsigned), void (*free)(void *))
{
	a->list = 0;
	a->base = base;
	a->count = 0;
	a->malloc = malloc;
	a->free = free;
}

void allocator_destroy(allocator *const a)
{
	while (a->list)
	{
		struct allocatorNode *aux = a->list->next;

		a->free(a->list);
		a->list = aux;
	}

	a->list = 0;
	a->base = 0;
	a->count = 0;
}

unsigned allocator_new(allocator *const a, unsigned chunk_size)
{
	unsigned pos = a->base;
	struct allocatorNode *node = a->list;
	struct allocatorNode *new = a->malloc(sizeof(struct allocatorNode));
	struct allocatorNode *next = 0;

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
		a->list = new;

	new->index = pos;
	new->size = chunk_size;
	new->next = next;

	++a->count;

	return pos;
}

void allocator_delete(allocator *const a, unsigned pos)
{
	if (pos < a->base)
		return;

	struct allocatorNode *node = a->list;
	struct allocatorNode *prev = 0;

	while (node)
	{
		if (node->index == pos)
		{
			prev->next = node->next;
			a->free(node);

			if (--a->count == 0)
				a->list = 0;

			return;
		}

		prev = node;
		node = node->next;
	}
}

// unsigned allocator_count(allocator *const a)
// {
// 	return a->count;
// }

// void allocator_info ()
//{
//	struct allocatorNode *aux = list;
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

// void allocator_sample ( void )
//{
//	allocator_init(0);
//
//	allocator_new(9);
//	s16 x = allocator_new(3);
//	allocator_new(927);
//	allocator_delete ( x );
//	allocator_new(2);
//	unsigned y = allocator_new(5);
//	allocator_new(1);
//	allocator_new(8);
//	allocator_delete ( y );
//	allocator_new(3);
//	allocator_new(2);
//	allocator_new(1);
//	allocator_new(3);
//
//	allocator_info();
// }
