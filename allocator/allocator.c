#include "allocator.h"
#include "config/free.h"
#include "config/malloc.h"

void allocator_init(Allocaltor *const a, unsigned int base)
{
	a->list = NULL;
	a->base = base;
	a->count = 0;
}

void allocator_destroy(Allocaltor *const a)
{
	while (a->list)
	{
		struct AllocaltorList *aux = a->list->next;

		free(a->list);
		a->list = aux;
	}

	a->list = NULL;
	a->base = 0;
	a->count = 0;
}

unsigned int allocator_new(Allocaltor *const a, unsigned int chunk_size)
{
	unsigned int pos = a->base;
	struct AllocaltorList *node = a->list;
	struct AllocaltorList *new = malloc(sizeof(struct AllocaltorList));
	struct AllocaltorList *next = NULL;

	if (node)
	{
		while (node)
		{
			pos = node->index + node->size;
			next = node->next;

			if (next == NULL || next->index >= pos + chunk_size)
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

void allocator_delete(Allocaltor *const a, unsigned int pos)
{
	if (pos < a->base)
		return;

	struct AllocaltorList *node = a->list;
	struct AllocaltorList *prev = NULL;

	while (node)
	{
		if (node->index == pos)
		{
			prev->next = node->next;
			free(node);

			if (--a->count == 0)
				a->list = NULL;

			return;
		}

		prev = node;
		node = node->next;
	}
}

// unsigned int allocator_count(Allocaltor *const a)
// {
// 	return a->count;
// }

// void allocator_info ()
//{
//	struct AllocaltorList *aux = list;
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
//	unsigned int y = allocator_new(5);
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
