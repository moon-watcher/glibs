#pragma once

struct allocatorNode
{
	unsigned int index;
	unsigned int size;
	struct allocatorNode *next;
};