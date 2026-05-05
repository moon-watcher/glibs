#pragma once

#include <stdint.h>

struct ffpAllocNode
{
	uint16_t index;
	uint16_t size;
	struct ffpAllocNode *next;
};

typedef struct
{
	struct ffpAllocNode *head;
	uint16_t base;
	uint16_t count;
} ffpAlloc_t;

uint16_t ffpAlloc_new(ffpAlloc_t *, uint16_t);
void ffpAlloc_delete(ffpAlloc_t *, uint16_t);
void ffpAlloc_destroy(ffpAlloc_t *);
