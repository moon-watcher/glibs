#pragma once

#include <stdint.h>

struct CG_RECT
{
    int16_t x1, y1, x2, y2;
};

struct CG_DEF
{
    struct CG_RECT rect;
    uint8_t capacity;
    uint8_t hCells;
    uint8_t vCells;
};

struct CG_CELL
{
    void **items;
    uint8_t size;
    uint8_t capacity;
};

typedef struct
{
    struct CG_CELL **cells;
    struct CG_RECT;
    struct CG_DEF;

    uint8_t *lookupTableCellX;
    uint8_t *lookupTableCellY;
} CollisionGrid, *pCollisionGrid;

//

#define CG_SIZE(def)                                        \
    sizeof(CollisionGrid) +                                 \
        def.vCells * sizeof(struct CG_CELL *) +             \
        (def.rect.x2 - def.rect.x1 + 1) * sizeof(uint8_t) + \
        (def.rect.y2 - def.rect.y1 + 1) * sizeof(uint8_t) + \
        def.vCells * def.hCells * sizeof(struct CG_CELL) +  \
        def.vCells * def.hCells * def.capacity * sizeof(void *)

//

void cg_init(pCollisionGrid, struct CG_DEF *);
struct CG_CELL *cg_get_CELL(pCollisionGrid, uint16_t, uint16_t);
struct CG_CELL *cg_addItem_FAST(pCollisionGrid, uint16_t, uint16_t, void *);
void cg_reset_CELLs(pCollisionGrid);
void cg_reset(pCollisionGrid);

void *cg_CELL_addItem(struct CG_CELL *, void *);
void *cg_CELL_removeItem(struct CG_CELL *, void *);

void cg_RECT_addItem(struct CG_CELL *[], uint16_t, void *);
uint16_t cg_RECT_getItems(struct CG_CELL *[], uint16_t, void *[]);
void cg_RECT_removeItem(struct CG_CELL *[], uint16_t, void *);

uint16_t cg_getItems_from_RECT(pCollisionGrid, struct CG_RECT *, void *[]);
uint16_t cg_RECT_collision_XY(struct CG_RECT *, uint16_t, uint16_t);

//
// uint16_t cg_get_RECT(pCollisionGrid, struct CG_RECT *, struct CG_CELL *[]);