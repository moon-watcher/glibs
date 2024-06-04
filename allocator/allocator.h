#pragma once

#include "node.h"

typedef struct
{
	struct allocatorNode *list;
	unsigned int base;
	unsigned int count;
} allocator;

void allocator_init(allocator *const, unsigned int);
void allocator_destroy(allocator *const);
unsigned int allocator_new(allocator *const, unsigned int);
void allocator_delete(allocator *const, unsigned int);