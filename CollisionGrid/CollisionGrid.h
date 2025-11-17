#pragma once

#include "config.h"

struct CG_RECT
{
    int16_t x, y, w, h;
};

struct CG_DEF
{
    struct CG_RECT rect;
    uint16_t capacity : CG_CELL_CAPACITY_BITS;
    uint16_t hCells : CG_DEF_HCELLS_BITS;
    uint16_t vCells : CG_DEF_VCELLS_BITS;
};

struct CG_CELL
{
    void **items;
    uint16_t size : CG_CELL_CAPACITY_BITS;
    uint16_t capacity : CG_CELL_CAPACITY_BITS;
};

typedef struct
{
    struct CG_CELL **cells;
    struct CG_DEF *def;
    uint8_t *lookupTableCellX;
    uint8_t *lookupTableCellY;
} CollisionGrid, *pCollisionGrid;

//

#define CG_SIZE(def)                                       \
    sizeof(CollisionGrid) +                                \
        def.vCells * sizeof(struct CG_CELL *) +            \
        (def.rect.w - def.rect.x + 1) * sizeof(uint8_t) +  \
        (def.rect.h - def.rect.y + 1) * sizeof(uint8_t) +  \
        def.vCells * def.hCells * sizeof(struct CG_CELL) + \
        def.vCells * def.hCells *def.capacity * sizeof(void *)

//

void cg_init(pCollisionGrid, struct CG_DEF *);
struct CG_CELL *cg_get_CELL(pCollisionGrid, uint16_t, uint16_t);
struct CG_CELL *cg_addItem_FAST(pCollisionGrid, uint16_t, uint16_t, void *);
void cg_reset_CELLs(pCollisionGrid);
void cg_reset(pCollisionGrid);

void *cg_CELL_addItem(struct CG_CELL *, void *);
uint16_t cg_CELL_removeItem(struct CG_CELL *, void *);

void cg_RECT_addItem(struct CG_CELL *[], uint16_t, void *);
uint16_t cg_RECT_getItems(struct CG_CELL *[], uint16_t, void *[]);
void cg_RECT_removeItem(struct CG_CELL *[], uint16_t, void *);

uint16_t cg_getItems_from_RECT(pCollisionGrid, struct CG_RECT *, void *[]);
uint16_t cg_RECT_collision_XY(struct CG_RECT *, uint16_t, uint16_t);

//
// uint16_t cg_get_RECT(pCollisionGrid, struct CG_RECT *, struct CG_CELL *[]);