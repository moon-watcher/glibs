#pragma once

typedef struct
{
	struct ffpAllocNode
	{
		unsigned index;
		unsigned size;
		struct ffpAllocNode *next;
	} *head;

	unsigned base;
	unsigned count;

	void *(*malloc)(unsigned);
	void (*free)(void *);
} ffpAlloc_t;

void ffpAlloc_init(ffpAlloc_t *const, void *(*)(unsigned), void (*)(void*));
void ffpAlloc_destroy(ffpAlloc_t *const);
unsigned ffpAlloc_new(ffpAlloc_t *const, unsigned);
void ffpAlloc_delete(ffpAlloc_t *const, unsigned);
