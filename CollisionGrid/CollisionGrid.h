#pragma once

#include <stdint.h>

struct CG_CELL
{
    void **items;
    uint8_t size;
    uint8_t capacity;
};

struct CG_RECT
{
    int16_t left, top;
    int16_t width, height;
};

struct CG_DEF
{
    struct CG_RECT;
    uint16_t capacity;
    uint16_t hCells;
    uint16_t vCells;
};

typedef struct CollisionGrid
{
    struct CG_CELL **cells;

    int16_t left, top;
    uint16_t width, height;
    uint16_t hCells, vCells;

    uint8_t *lookupTableCellX;
    uint8_t *lookupTableCellY;
} CollisionGrid, *pCollisionGrid;

//

#define CG_SIZE(def)                                       \
    sizeof(CollisionGrid) +                                \
        def.vCells * sizeof(struct CG_CELL *) +            \
        def.width * sizeof(uint8_t) +                      \
        def.height * sizeof(uint8_t) +                     \
        def.vCells * def.hCells * sizeof(struct CG_CELL) + \
        def.vCells * def.hCells * def.capacity * sizeof(void *)

void cg_init(pCollisionGrid, struct CG_DEF *);
struct CG_CELL *cg_cellGet_XY(pCollisionGrid, uint16_t, uint16_t);
uint16_t cg_itemsGet_RECT(pCollisionGrid, struct CG_RECT *, void *[]);
void cg_resetCells(pCollisionGrid);
void cg_reset(pCollisionGrid);

uint16_t cg_cell_itemAdd(struct CG_CELL *, void *);
uint16_t cg_cell_itemRemove(struct CG_CELL *, void *);

#define CG_RECT_COLLISION_XY(rect, x, y) \
    (((x) - (rect)->left) < (rect)->width && ((y) - (rect)->top) < (rect)->height)
