#pragma once

typedef struct
{
    unsigned hCells, vCells;
    unsigned totalBytes;

    struct CG_RECT
    {
        int left, top;
        int right, bottom;
    } area;

    struct CG_CELL
    {
        void **items;
        unsigned char size;
        unsigned char capacity;
    } **cells;

    unsigned char *lookupTableCellX;
    unsigned char *lookupTableCellY;
} CollisionGrid;

//

CollisionGrid      *cg_init(struct CG_RECT, unsigned, unsigned, unsigned);
void                cg_reset(CollisionGrid *const);
void                cg_end(CollisionGrid *);

struct CG_CELL *cg_CELL_get(CollisionGrid *const, unsigned, unsigned);
void               *cg_CELL_addItem(struct CG_CELL *const, void *const);
unsigned            cg_CELL_removeItem(struct CG_CELL *const, void *const);

unsigned            cg_RECT_get(CollisionGrid *const, struct CG_RECT *const, struct CG_CELL *[]);
void                cg_RECT_addItem(struct CG_CELL *[], unsigned, void *const);
unsigned            cg_RECT_getItems(struct CG_CELL *[], unsigned, void *[]);
void                cg_RECT_removeItem(struct CG_CELL *[], unsigned, void *const);
