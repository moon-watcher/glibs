#pragma once

struct CG_CELL
{
    void **items;
    unsigned char size : 4;
    unsigned char capacity : 4;
};

typedef struct
{
    struct CG_CELL **cells;

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
    } const *def;

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

void cg_init(CollisionGrid *const, const struct CG_DEF *);
struct CG_CELL *cg_get_CELL(CollisionGrid *const, unsigned, unsigned);
struct CG_CELL *cg_addItem_FAST(CollisionGrid *const, unsigned, unsigned, void *const);
void cg_reset_CELLs(CollisionGrid *const);
// unsigned cg_get_RECT(CollisionGrid *const, struct CG_RECT *const, struct CG_CELL *[]);
void cg_reset(CollisionGrid *const);

void *cg_CELL_addItem(struct CG_CELL *const, void *const);
unsigned cg_CELL_removeItem(struct CG_CELL *const, void *const);

void cg_RECT_addItem(struct CG_CELL *[], unsigned, void *const);
unsigned cg_RECT_getItems(struct CG_CELL *[], unsigned, void *[]);
void cg_RECT_removeItem(struct CG_CELL *[], unsigned, void *const);

unsigned cg_getItems_from_RECT(CollisionGrid *const, const struct CG_RECT *, void *[]);
unsigned cg_RECT_collision_XY(struct CG_RECT *const, unsigned, unsigned);
