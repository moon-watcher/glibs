#pragma once

#include <stdint.h>

struct CG_CELL
{
    void **items;
    unsigned char size : 4;
    unsigned char capacity : 4;
};

struct CG_DEF
{
    struct CG_RECT
    {
        int x, y;
        int right, bottom;
    };
    unsigned capacity;
    unsigned hCells;
    unsigned vCells;
};

typedef struct
{
    struct CG_CELL **cells;
    struct CG_DEF *def;
    unsigned char *lookupTableCellX;
    unsigned char *lookupTableCellY;
} CollisionGrid, *pCollisionGrid;

//

#define CG_SIZE(def)                                       \
    sizeof(CollisionGrid) +                                \
        def.vCells * sizeof(struct CG_CELL *) +            \
        (def.right - def.x + 1) * sizeof(unsigned char) +  \
        (def.bottom - def.y + 1) * sizeof(unsigned char) + \
        def.vCells *def.hCells * sizeof(struct CG_CELL) +  \
        def.vCells *def.hCells *def.capacity * sizeof(void *)

//

void cg_init(pCollisionGrid, struct CG_DEF *);
struct CG_CELL *cg_get_CELL(pCollisionGrid, unsigned, unsigned);
struct CG_CELL *cg_addItem_FAST(pCollisionGrid, unsigned, unsigned, void *);
void cg_reset_CELLs(pCollisionGrid);
void cg_reset(pCollisionGrid);

void *cg_CELL_addItem(struct CG_CELL *, void *);
unsigned cg_CELL_removeItem(struct CG_CELL *, void *);

void cg_RECT_addItem(struct CG_CELL *[], unsigned, void *);
unsigned cg_RECT_getItems(struct CG_CELL *[], unsigned, void *[]);
void cg_RECT_removeItem(struct CG_CELL *[], unsigned, void *);

unsigned cg_getItems_from_RECT(pCollisionGrid, struct CG_RECT *, void *[]);
unsigned cg_RECT_collision_XY(struct CG_RECT *, unsigned, unsigned);

//
// unsigned cg_get_RECT(pCollisionGrid, struct CG_RECT *, struct CG_CELL *[]);