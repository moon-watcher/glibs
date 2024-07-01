#pragma once

typedef struct
{
	struct allocatorNode
	{
		unsigned index;
		unsigned size;
		struct allocatorNode *next;
	} *list;

	unsigned base;
	unsigned count;
} allocator;

void allocator_init(allocator *const, unsigned);
void allocator_destroy(allocator *const);
unsigned allocator_new(allocator *const, unsigned);
void allocator_delete(allocator *const, unsigned);