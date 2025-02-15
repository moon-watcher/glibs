#pragma once

typedef struct
{
    struct CG_DEF
    {
        struct CG_RECT
        {
            int left, top;
            int right, bottom;
        };
        unsigned capacity;
        unsigned hCells;
        unsigned vCells;
    };

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

#define CG_SIZE(def)                                         \
    sizeof(CollisionGrid) +                                  \
        def.vCells * sizeof(struct CG_CELL *) +              \
        (def.right - def.left + 1) * sizeof(unsigned char) + \
        (def.bottom - def.top + 1) * sizeof(unsigned char) + \
        def.vCells *def.hCells * sizeof(struct CG_CELL) +    \
        def.vCells *def.hCells *def.capacity * sizeof(void *)

//

void cg_init(CollisionGrid *const, struct CG_DEF *const);
struct CG_CELL *cg_get_CELL(CollisionGrid *const, unsigned, unsigned);
unsigned cg_get_RECT(CollisionGrid *const, struct CG_RECT *const, struct CG_CELL *[]);
void cg_reset(CollisionGrid *const);

void *cg_CELL_addItem(struct CG_CELL *const, void *const);
unsigned cg_CELL_removeItem(struct CG_CELL *const, void *const);

void cg_RECT_addItem(struct CG_CELL *[], unsigned, void *const);
unsigned cg_RECT_getItems(struct CG_CELL *[], unsigned, void *[]);
void cg_RECT_removeItem(struct CG_CELL *[], unsigned, void *const);
