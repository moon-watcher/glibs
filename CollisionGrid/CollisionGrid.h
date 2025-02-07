#pragma once

typedef struct
{
    int left;
    int top;
    int right;
    int bottom;
} CollisionGrid_RECT;

typedef struct
{
    void **items;
    unsigned char size;
    unsigned char capacity;
} CollisionGrid_CELL;

typedef struct
{
    unsigned hCells;
    unsigned vCells;
    unsigned totalBytes;
    CollisionGrid_RECT area;
    CollisionGrid_CELL **cells;
    unsigned char *lookupTableCellX;
    unsigned char *lookupTableCellY;
} CollisionGrid;

//

CollisionGrid      *cg_init(CollisionGrid_RECT, unsigned, unsigned, unsigned);
void                cg_reset(CollisionGrid *const);
void                cg_end(CollisionGrid *);

CollisionGrid_CELL *cg_CELL_get(CollisionGrid *const, unsigned, unsigned);
void               *cg_CELL_addItem(CollisionGrid_CELL *const, void *const);
unsigned            cg_CELL_removeItem(CollisionGrid_CELL *const, void *const);

unsigned            cg_RECT_get(CollisionGrid *const, CollisionGrid_RECT *const, CollisionGrid_CELL *[]);
void                cg_RECT_addItem(CollisionGrid_CELL *[], unsigned, void *const);
unsigned            cg_RECT_getItems(CollisionGrid_CELL *[], unsigned, void *[]);
void                cg_RECT_removeItem(CollisionGrid_CELL *[], unsigned, void *const);
