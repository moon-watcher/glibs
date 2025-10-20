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

	void *(*malloc)(unsigned);
	void (*free)(void *);
} allocator;

void allocator_init(allocator *const, unsigned, void *(*)(unsigned), void (*)(void*));
void allocator_destroy(allocator *const);
unsigned allocator_new(allocator *const, unsigned);
void allocator_delete(allocator *const, unsigned);
