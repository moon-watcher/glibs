#pragma once

struct CG_RECT
{
    int left, top;
    int right, bottom;
};

struct CG_DEF
{
    struct CG_RECT gridArea;
    unsigned capacity;
    unsigned hCells;
    unsigned vCells;
};

typedef struct
{
    unsigned hCells, vCells;

    struct CG_RECT area;

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

unsigned cg_size(struct CG_DEF *const def);
void cg_init(CollisionGrid * cg, struct CG_RECT gridArea, unsigned items, unsigned hCells, unsigned vCells);
void cg_reset(CollisionGrid *const);

struct CG_CELL *cg_CELL_get(CollisionGrid *const, unsigned, unsigned);
void *cg_CELL_addItem(struct CG_CELL *const, void *const);
unsigned cg_CELL_removeItem(struct CG_CELL *const, void *const);

unsigned cg_RECT_get(CollisionGrid *const, struct CG_RECT *const, struct CG_CELL *[]);
void cg_RECT_addItem(struct CG_CELL *[], unsigned, void *const);
unsigned cg_RECT_getItems(struct CG_CELL *[], unsigned, void *[]);
void cg_RECT_removeItem(struct CG_CELL *[], unsigned, void *const);
