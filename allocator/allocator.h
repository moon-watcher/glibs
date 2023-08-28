#pragma once

typedef struct
{
	struct AllocaltorList
	{
		unsigned int index;
		unsigned int size;
		struct AllocaltorList *next;
	} *list;

	unsigned int base;
	unsigned int count;
} Allocaltor;

void allocator_init(Allocaltor *const, unsigned int);
void allocator_destroy(Allocaltor *const);
unsigned int allocator_new(Allocaltor *const, unsigned int);
void allocator_delete(Allocaltor *const, unsigned int);
// unsigned int allocator_count(Allocaltor *const);