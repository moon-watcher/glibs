#pragma once

typedef struct
{
	struct allocatorList
	{
		unsigned int index;
		unsigned int size;
		struct allocatorList *next;
	} *list;

	unsigned int base;
	unsigned int count;
} allocator;

void allocator_init(allocator *const, unsigned int);
void allocator_destroy(allocator *const);
unsigned int allocator_new(allocator *const, unsigned int);
void allocator_delete(allocator *const, unsigned int);
// unsigned int allocator_count(allocator *const);