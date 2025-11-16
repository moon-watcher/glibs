//  #include <genesis.h>
#include "CollisionGrid.h"


void cg_init(pCollisionGrid this, struct CG_DEF *def)
{
    unsigned width  = def->right  - def->x + 1;
    unsigned height = def->bottom - def->y + 1;
    unsigned cap    = def->capacity;
    unsigned hCells = def->hCells;
    unsigned vCells = def->vCells;
    unsigned wh     = (width  + hCells - 1) / hCells;
    unsigned hv     = (height + vCells - 1) / vCells;

    this->def              = def;
    this->cells            = (struct CG_CELL **)((char *)this + sizeof(CollisionGrid));
    this->lookupTableCellX = (unsigned char *)((char *)this->cells + vCells * sizeof(struct CG_CELL *));
    this->lookupTableCellY = (unsigned char *)((char *)this->lookupTableCellX + width);

    for (unsigned i = 0; i < width;  i++) this->lookupTableCellX[i] = i / wh;
    for (unsigned i = 0; i < height; i++) this->lookupTableCellY[i] = i / hv;

    struct CG_CELL *cellMemory = (struct CG_CELL *)((char *)this->lookupTableCellY + height);

    for (unsigned y = 0; y < vCells; ++y)
    {
        this->cells[y] = &cellMemory[y * hCells];

        for (unsigned x = 0; x < hCells; ++x)
        {
            struct CG_CELL *cell = &this->cells[y][x];
            cell->items = (void **)((char *)cellMemory + vCells * hCells * sizeof(struct CG_CELL) + (y * hCells + x) * cap * sizeof(void *));
            cell->size = 0;
            cell->capacity = cap;
        }
    }
}

inline struct CG_CELL *cg_get_CELL(pCollisionGrid this, unsigned x, unsigned y)
{
    struct CG_DEF *def = this->def;
    
    int a_left = def->x;
    if (x < a_left)
        return 0;

    int a_top = def->y;
    if (y < a_top)
        return 0;
    
    unsigned char cellX = this->lookupTableCellX[x - a_left];    
    if (cellX >= def->hCells)
        return 0;

    unsigned char cellY = this->lookupTableCellY[y - a_top];
    if (cellY >= def->vCells)
        return 0;
        
    return &this->cells[cellY][cellX];
}

// unsigned cg_get_RECT(pCollisionGrid this, struct CG_RECT *rect, struct CG_CELL *cell_list[])
// {
//     int left = rect->x;
//     int right = rect->right;
//     if (left >= right) return 0;

//     int top = rect->y;
//     int bottom = rect->bottom;
//     if (top >= bottom) return 0;

//     unsigned count = 0;
//     int this_left = this->x;
//     int this_top = this->y;
//     unsigned hCells = this->hCells;
//     unsigned vCells = this->vCells;
//     unsigned char cellX_min = this->lookupTableCellX[left - this_left];
//     unsigned char cellY_min = this->lookupTableCellY[top - this_top];
//     unsigned cellX_max = this->lookupTableCellX[right - this_left];
//     unsigned cellY_max = this->lookupTableCellY[bottom - this_top];

//     if (cellX_max >= hCells) cellX_max = hCells - 1;
//     if (cellY_max >= vCells) cellY_max = vCells - 1;

//     for (unsigned char y = cellY_min; y <= cellY_max; ++y)
//         for (unsigned char x = cellX_min; x <= cellX_max; ++x)
//             cell_list[count++] = &this->cells[y][x];

//     return count;
// }

inline struct CG_CELL *cg_addItem_FAST(pCollisionGrid this, unsigned x, unsigned y, void *item)
{
    struct CG_DEF *def = this->def;
    unsigned char cellX = this->lookupTableCellX[x - def->x];
    unsigned char cellY = this->lookupTableCellY[y - def->y];
    struct CG_CELL *cell = &this->cells[cellY][cellX];

    cell->items[cell->size++] = item;

    return cell;
}

void cg_reset_CELLs(pCollisionGrid this)
{
    struct CG_DEF *def = this->def;
    unsigned vCells = def->vCells;
    unsigned hCells = def->hCells;

    for (unsigned cellY = 0; cellY < vCells; ++cellY)
        for (unsigned cellX = 0; cellX < hCells; ++cellX)
            this->cells[cellY][cellX].size = 0;
}

void cg_reset(pCollisionGrid this)
{
    struct CG_DEF *def = this->def;
    unsigned vCells = def->vCells;
    unsigned hCells = def->hCells;

    memset(this->cells, 0, vCells * hCells * sizeof(struct CG_CELL));
}

//

inline void *cg_CELL_addItem(struct CG_CELL *this, void *item)
{
    if (this->size >= this->capacity)
        return 0;
        
    return this->items[this->size++] = item;
}

unsigned cg_CELL_removeItem(struct CG_CELL *this, void *item)
{
    unsigned char size = this->size;

    for (unsigned i = 0; i < size; i++)
        if (this->items[i] == item)
        {
            this->items[i] = this->items[--this->size];
            return 1;
        }

    return 0;
}

//

void cg_RECT_addItem(struct CG_CELL *cell_list[], unsigned total, void *item)
{
    for (unsigned i = 0; i < total; i++)
        cg_CELL_addItem(cell_list[i], item);
}

unsigned cg_RECT_getItems(struct CG_CELL *cell_list[], unsigned total, void *item_list[])
{
    unsigned count = 0;

    for (unsigned i = 0; i < total; i++)
    {
        struct CG_CELL *cell = cell_list[i];
        unsigned char size = cell->size;

        for (unsigned j = 0; j < size; j++)
            item_list[count++] = cell->items[j];
    }

    return count;
}

void cg_RECT_removeItem(struct CG_CELL *cell_list[], unsigned total, void *item)
{
    for (unsigned i = 0; i < total; i++)
        cg_CELL_removeItem(cell_list[i], item);
}

unsigned cg_getItems_from_RECT(pCollisionGrid this, struct CG_RECT *rect, void *item_list[])
{
    int left  = rect->x;
    int right = rect->right;
    if (left >= right)
        return 0;

    int top    = rect->y;
    int bottom = rect->bottom;
    if (top >= bottom)
        return 0;

    unsigned count           = 0;
    struct CG_DEF *def = this->def;
    int this_left      = def->x;
    int this_top       = def->y;
    unsigned hCells    = def->hCells;
    unsigned vCells    = def->vCells;
    unsigned cellX_min = this->lookupTableCellX[left   - this_left];
    unsigned cellY_min = this->lookupTableCellY[top    - this_top];
    unsigned cellX_max       = this->lookupTableCellX[right  - this_left];
    unsigned cellY_max       = this->lookupTableCellY[bottom - this_top];

    if (cellX_max >= hCells) cellX_max = hCells - 1;
    if (cellY_max >= vCells) cellY_max = vCells - 1;

    for (unsigned y = cellY_min; y <= cellY_max; ++y)
        for (unsigned x = cellX_min; x <= cellX_max; ++x)
        {
            struct CG_CELL *cell = &this->cells[y][x];
            unsigned size = cell->size;
            void **src = cell->items;

            for (unsigned i = 0; i < size; ++i)
                item_list[count++] = src[i];
        }

    return count;
}

inline unsigned cg_RECT_collision_XY(struct CG_RECT *rect, unsigned x, unsigned y)
{
    return (x >= rect->x && x <= rect->right && y >= rect->y && y <= rect->bottom);
}
